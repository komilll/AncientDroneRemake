#include "enemyflying.h"

EnemyFlying::EnemyFlying()
{
	EnemyBase::EnemyBase();
	gravity = 0.0f;
	useGravity = false;
	m_bomb = 0;
}

EnemyFlying::EnemyFlying(const EnemyFlying &)
{
}

EnemyFlying::~EnemyFlying()
{
}

bool EnemyFlying::Init(GraphicsClass* graphicsClass, float width, float height, float translationX, float translationY)
{
	m_bomb = new FlyingEnemyBomb(graphicsClass, 4, 1.0f);	
	return EnemyBase::Init(graphicsClass, width, height, translationX, translationY);
}

void EnemyFlying::Update()
{
	EnemyBase::Update();


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

	Move(frameMovementRight);

	if (m_bomb)
	{
		m_bomb->Update();
		m_bomb->TouchedPlayer(player, player->GetBounds().min.x, player->GetBounds().max.x, player->GetBounds().min.y, player->GetBounds().max.y);
	}
}

void EnemyFlying::Move(float x)
{
	EnemyBase::Move(x);
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
