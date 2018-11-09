#include "enemyarcher.h"

EnemyArcher::EnemyArcher()
{
}

EnemyArcher::EnemyArcher(const EnemyArcher &)
{
}

EnemyArcher::~EnemyArcher()
{
}

bool EnemyArcher::Init(GraphicsClass * graphicsClass, float width, float height, float translationX, float translationY)
{
	for (int i = 0; i < 5; i++)
	{
		m_arrow.push_back(new EnemyArcherArrow());
	}
	return EnemyBase::Init(graphicsClass, width, height, translationX, translationY);
}

void EnemyArcher::Update()
{
	EnemyBase::Update();
}

void EnemyArcher::FixedUpdate()
{
	EnemyBase::FixedUpdate();
}

void EnemyArcher::SetPlayer(Player * player_)
{
	player = player_;
}