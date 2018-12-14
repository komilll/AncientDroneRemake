#include "dronecontroller.h"

DroneController::DroneController()
{
	MovingObjectPrototype::MovingObjectPrototype();

	speed = 5.0f;
	m_droneOnPlayer = true;
}

bool DroneController::Init(GraphicsClass * graphicsClass, float width, float height, float translationX, float translationY, CHAR * animationSheetName)
{
	bool toReturn = MovingObjectPrototype::Init(graphicsClass, width, height, translationX, translationY, "ancient_ball.dds");
	if (!toReturn)
		return false;

	for (int i = 0; i < 5; i++)
	{
		m_spears.push_back(new DroneSpearClass());
		m_spears.at(i)->Init(m_graphics, 8.0f, 8.0f, 0.0f, 0.0f, "mob_archer.dds");		
	}

	m_shader->AddModel(m_model);
	//Animations
	m_animation = new PlayerAnimationStates(2);
	m_shader->ImportFile(64, 64, 1024, 1024);
	m_shader->CreateNewAnimation(1, 10, 0); //IDLE
	m_shader->CreateNewAnimation(6, 5, 0, false); //ATTACKING

	m_animation->PrepareAnimationPose(IDLE, IDLE);	
	m_animation->PrepareAnimationPose(ATTACKING, ATTACKING);
	m_animation->SetState(IDLE);
	SetNewAnimation(IDLE);

	m_wander = false;
	useGravity = false;

	m_model->SetBounds(-width, width, -height, height);
	
	//Explosion Initiation
	m_explosionModel = new ModelClass();
	m_explosionModel->Initialize(m_graphics->GetD3D()->GetDevice(), m_startSizeExplosion, m_startSizeExplosion);

	m_explosionShader = new TextureShaderGeneralClass();
	if (!m_explosionShader)
		return false;

	if (!m_explosionShader->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND(), "DarkSphere_Core.dds", "droneexplosion.vs", "droneexplosion.ps"))
		return false;

	if (!m_graphics->AddTextureShaderGeneral(m_explosionShader))
	{
		m_explosionShader->Shutdown();
		delete m_explosionShader;
		m_explosionShader = 0;
	}

	m_explosionShader->AddModel(m_explosionModel);
	m_explosionShader->SetAsTransparent(true);
	m_explosionModel->SetVisibility(false);

	return toReturn;
}

void DroneController::Update()
{
	MovingObjectPrototype::Update();
	for (int i = 0; i < m_spears.size(); i++)
		m_spears.at(i)->Update();
}

void DroneController::FixedUpdate()
{
	if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - m_lastAttackTime > m_timeToRecoverEnergy)
	{
		m_energyLeft += 1;
		//m_energyLeft = min(m_energyLeft, m_energyMax);
		if (m_energyLeft > m_energyMax)
			m_energyLeft = m_energyMax;
	}

	m_attackCooldownCurrent = max(m_attackCooldownCurrent - 0.02f, 0.0f);

	//MovingObjectPrototype::FixedUpdate();
	float distance = abs(m_destX - m_posX());
	if ((distance < m_epsilon && m_droneForceToPlayer == false) || (distance < m_epsilonDistance && m_droneForceToPlayer == true))
	{
		//Snap to correct position
		m_model->SetTranslation(m_destX, m_destY, 0.0f);	
		if (m_droneForceToPlayer)
			m_droneOnPlayer = true;
		m_droneForceToPlayer = false;
	}
	else
	{		
		m_model->SetTranslation(m_model->GetTranslation().x + (m_destX - m_posX()) * speed * 0.02f,
			m_model->GetTranslation().y + (m_destY - m_posY()) * speed * 0.02f, 0.0f);
	}

	if (m_model->GetTranslation().x < m_graphics->GetPlayerPosition().x - m_boundsScreenX ||
		m_model->GetTranslation().x > m_graphics->GetPlayerPosition().x + m_boundsScreenX ||
		m_model->GetTranslation().y < m_graphics->GetPlayerPosition().y - m_boundsScreenY ||
		m_model->GetTranslation().y > m_graphics->GetPlayerPosition().y + m_boundsScreenY)
	{
		DroneOutsideScreen();
	}

	if (m_droneForceToPlayer)
	{
		m_destX = m_graphics->GetPlayerPosition().x + m_distToPlayerX;
		m_destY = m_graphics->GetPlayerPosition().y + m_distToPlayerY;
	}

	if (m_droneOnPlayer)
	{
		if (m_graphics->GetPlayerModel()->movingRight)
			SetDestination(m_graphics->GetPlayerPosition().x + m_distToPlayerX, m_graphics->GetPlayerPosition().y + m_distToPlayerY);
		else
			SetDestination(m_graphics->GetPlayerPosition().x - m_distToPlayerX, m_graphics->GetPlayerPosition().y + m_distToPlayerY);
	}

	if (m_explosionModel)
	{
		m_explosionModel->SetTranslation(m_model->GetTranslation().x, m_model->GetTranslation().y, m_model->GetTranslation().z);
		if (m_isExploding)
		{
			m_currentExplodeTime += 20;
			float scale = Lerp(m_startSizeExplosion, m_endSizeExplosion, (float)m_currentExplodeTime / (float)m_timeToExplode) / m_startSizeExplosion;
			m_explosionModel->SetScale(scale, scale, scale);
			if (m_currentExplodeTime >= m_timeToExplode)
			{
				m_explosionModel->SetVisibility(false);
				m_isExploding = false;
				m_currentExplodeTime = 0;
			}
		}
	}
}

void DroneController::Move(float x)
{
	MovingObjectPrototype::Move(x);
}

bool DroneController::TouchedPlayer(Player * player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY)
{
	return false;
}

void DroneController::SetNewAnimation(int newState)
{
	MovingObjectPrototype::SetNewAnimation(newState);
}

void DroneController::PlayOneShotAnimation(int state, int previousState)
{
	MovingObjectPrototype::PlayOneShotAnimation(state, previousState);
}

void DroneController::HitedWall()
{
	return;
}

bool DroneController::Attack()
{
	if (m_attackCooldownCurrent > 0.0f)
		return false;

	if (m_energyLeft < m_bulletEnergyCost)
		return false;

	m_lastAttackTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	m_energyLeft -= m_bulletEnergyCost;

	PlayOneShotAnimation(ATTACKING);
	m_attackCooldownCurrent = m_attackCooldown;

	m_spears.at(m_spearIndex)->GetModel()->SetTranslation(m_model->GetTranslation().x + (m_model->GetSize().x * Forward().x), 
		m_model->GetTranslation().y + (m_model->GetSize().y * Forward().y), 0.0f);
	m_spears.at(m_spearIndex)->GetModel()->SetRotation(m_model->GetRotation());
	m_spears.at(m_spearIndex)->Spawn();
	m_spearIndex = (m_spearIndex + 1) % m_spears.size();

	return true;
}

void DroneController::SetDestination(float destX, float destY)
{
	m_destX = destX;
	m_destY = destY;
}

void DroneController::CheckSpearsDamage(MovingObjectPrototype *object)
{
	for (int i = 0; i < m_spears.size(); i++)
	{
		if (m_spears.at(i)->TouchedEnemy(object))
		{
			//Hitted enemy, handled in spear script
		}
	}

	ExplosionTouchedEnemy(object);
}

float DroneController::GetDroneEnergyProgress()
{
	return (float)m_energyLeft / (float)m_energyMax;
}

void DroneController::DroneOutsideScreen()
{
	m_droneForceToPlayer = true;
	m_destX = m_graphics->GetPlayerPosition().x + m_distToPlayerX;
	m_destY = m_graphics->GetPlayerPosition().y + m_distToPlayerY;
}

void DroneController::FreeDroneFromPlayer()
{
	m_droneOnPlayer = false;
	m_droneForceToPlayer = false;
}

void DroneController::CallDroneToPlayer()
{
	m_droneForceToPlayer = true;
}

void DroneController::SetDroneFullEnergy()
{
	m_energyLeft = m_energyMax;
}

void DroneController::AttackExplosion()
{
	if (m_attackCooldownCurrent > 0.0f || m_energyLeft < m_explosionEnergyCost)
		return;

	m_damageByExplosionEnemies.clear();
	m_explosionModel->SetVisibility(true);
	m_isExploding = true;
	m_currentExplodeTime = 0;
	m_attackCooldownCurrent = m_attackExplosionCooldown;
	m_energyLeft -= m_explosionEnergyCost;
	m_lastAttackTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

bool DroneController::ExplosionTouchedEnemy(MovingObjectPrototype *object)
{
	if (!m_isExploding || object == nullptr)
		return false;

	for (int i = 0; i < m_damageByExplosionEnemies.size(); i++)
		if (m_damageByExplosionEnemies.at(i) == object)
			return false;

	m_damageByExplosionEnemies.push_back(object);

	ModelClass* enemyModel = object->GetModel();
	if ((m_explosionModel->GetBounds().min.x < enemyModel->GetBounds().max.x && enemyModel->GetBounds().max.x < m_explosionModel->GetBounds().max.x) || //Enter from the left side		
		(m_explosionModel->GetBounds().max.x > enemyModel->GetBounds().min.x && enemyModel->GetBounds().min.x > m_explosionModel->GetBounds().min.x)) //Enter from the right side
	{
		if (enemyModel->GetBounds().min.y < m_explosionModel->GetBounds().max.y && enemyModel->GetBounds().min.y > m_explosionModel->GetBounds().max.y || //Enter from the bottom
			enemyModel->GetBounds().max.y > m_explosionModel->GetBounds().min.y && m_explosionModel->GetBounds().max.y > enemyModel->GetBounds().min.y) //Enter from the top
		{
			if (object->DamageObject(2))
				return true;
			else
				return false;
		}
	}

	return false;
}