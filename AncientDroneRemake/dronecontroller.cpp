#include "dronecontroller.h"

DroneController::DroneController()
{
	MovingObjectPrototype::MovingObjectPrototype();

	speed = 5.0f;
}

bool DroneController::Init(GraphicsClass * graphicsClass, float width, float height, float translationX, float translationY, CHAR * animationSheetName)
{
	bool toReturn = MovingObjectPrototype::Init(graphicsClass, width, height, translationX, translationY, "ancient_ball.dds");
	if (!toReturn)
		return false;

	for (int i = 0; i < 1; i++)
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
	//m_model->SetRotation(90.0f);	

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
	m_attackCooldownCurrent = max(m_attackCooldownCurrent - 0.02f, 0.0f);

	//MovingObjectPrototype::FixedUpdate();
	float distance = abs(m_destX - m_posX());
	if (distance < m_epsilon)
	{
		//Do nothing
	}
	else if (distance < m_epsilonDistance)
	{
		//Snap to correct position
		m_model->SetTranslation(m_destX, m_destY, 0.0f);
	}
	else
	{		
		m_model->SetTranslation(m_model->GetTranslation().x + (m_destX - m_posX()) * speed * 0.02f,
			m_model->GetTranslation().y + (m_destY - m_posY()) * speed * 0.02f, 0.0f);
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

	PlayOneShotAnimation(ATTACKING);
	m_attackCooldownCurrent = m_attackCooldown;
	m_spears.at(0)->GetModel()->SetTranslation(m_model->GetBounds().max.x, m_model->GetTranslation().y, 0.0f);
	//m_spears.at(0)->GetModel()->SetRotation(m_model->GetRotation());
	m_spears.at(0)->Spawn();
	//TODO Check if attack is not on cooldown
	return true;
}

void DroneController::SetDestination(float destX, float destY)
{
	m_destX = destX;
	m_destY = destY;
}