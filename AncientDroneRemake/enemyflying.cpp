#include "enemyflying.h"

EnemyFlying::EnemyFlying()
{
	EnemyBase::EnemyBase();
	gravity = 0.0f;
	useGravity = false;
}

EnemyFlying::EnemyFlying(const EnemyFlying &)
{
}

EnemyFlying::~EnemyFlying()
{
}

bool EnemyFlying::Init(GraphicsClass* graphicsClass, float width, float height, float translationX, float translationY, CHAR* animationSheetName)
{
	for (int i = 0; i < 5; i++)
	{
		m_bomb.push_back(new FlyingEnemyBomb(graphicsClass, 4, 0.35f));
	}
	bool toReturn = EnemyBase::Init(graphicsClass, width, height, translationX, translationY, "mob_crow.dds");
	if (!toReturn)
		return false;

	m_shader->AddModel(m_model);
	//Animations
	m_animation = new PlayerAnimationStates(2);
	m_shader->ImportFile(128, 128, 1024, 1024);
	m_shader->CreateNewAnimation(1, 10, 0); //IDLE
	m_shader->CreateNewAnimation(6, 5, 0); //MOVING
	m_shader->CreateNewAnimation(3, 5, 1, false); //ATTACKING

	m_animation->PrepareAnimationPose(IDLE, IDLE);
	m_animation->PrepareAnimationPose(MOVING, MOVING);
	m_animation->PrepareAnimationPose(ATTACKING, ATTACKING);
	m_animation->SetState(IDLE);
	SetNewAnimation(MOVING);

	m_wander = true;

	m_model->SetBounds(-width, width, -height + 2, height);

	return toReturn;
}

void EnemyFlying::Update()
{
	EnemyBase::Update();

	Move(frameMovementRight);

	if (m_destroyed)
	{
		__int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		timer += (now - lastTime);
		lastTime = now;

		if (timer >= 20.0f) //20ms = 0.02s		
			UpdateBombs();		
	}
}

void EnemyFlying::FixedUpdate()
{
	UpdateBombs();

	EnemyBase::FixedUpdate();
	
	m_bombCooldownCurrent += 0.02f;
	if (m_bombCooldownCurrent >= m_bombCooldownTime)
	{
		m_bombCooldownCurrent = 0.0f;

		m_bombIndex = m_bombIndex % m_bomb.size();
		m_bomb[m_bombIndex]->Init(m_model->GetTranslation().x, m_model->GetTranslation().y - m_model->GetSize().y * .5f - m_bomb[m_bombIndex]->GetModel()->GetSize().y * .5f);
		m_bombIndex++;
	}

	if (m_model->GetBounds().max.x > rightWaypoint.x && frameMovementRight > 0.0f)
	{
		speed = -abs(speed);
		frameMovementRight = speed;
	}
	else if (m_model->GetBounds().min.x < leftWaypoint.x && frameMovementRight < 0.0f)
	{
		speed = abs(speed);
		frameMovementRight = speed;
	}

}

void EnemyFlying::Move(float x)
{
	EnemyBase::Move(x);
}

void EnemyFlying::Shutdown()
{
	EnemyBase::Shutdown();
	for (int i = 0; i < m_bomb.size(); i++)
	{
		m_bomb.at(i)->Shutdown();
		delete m_bomb.at(i);
	}
	m_bomb.clear();
	m_graphics->ClearBombs();
}

void EnemyFlying::SetPlayer(Player * player_)
{
	player = player_;
}

void EnemyFlying::SetWaypoints(D3DXVECTOR2 waypoint1, D3DXVECTOR2 waypoint2)
{
	if (waypoint1.x > waypoint2.x)
	{
		leftWaypoint = waypoint2;
		rightWaypoint = waypoint1;
	}
	else if (waypoint1.x < waypoint2.x)
	{
		rightWaypoint = waypoint2;
		leftWaypoint = waypoint1;
	}
	else
	{
		MessageBox(*m_graphics->GetHWND(), "Waypoint x positions are the same", "EnemyFlying.cpp", MB_OK);
	}

}

void EnemyFlying::UpdateBombs()
{
	for (std::vector<FlyingEnemyBomb*>::iterator bombIter = m_bomb.begin(); bombIter != m_bomb.end(); ++bombIter)
	{
		FlyingEnemyBomb* ptr = *(bombIter._Ptr);
		if (ptr != nullptr)
		{
			ptr->Update();
			ptr->TouchedPlayer(player, player->GetBounds().min.x, player->GetBounds().max.x, player->GetBounds().min.y, player->GetBounds().max.y);
		}
	}
}
