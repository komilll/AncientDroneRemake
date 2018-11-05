#include "enemywanderer.h"

EnemyWanderer::EnemyWanderer()
{
	EnemyBase::EnemyBase();
	gravity = 15.0f;
}

EnemyWanderer::EnemyWanderer(const EnemyWanderer &)
{
}

EnemyWanderer::~EnemyWanderer()
{
}

bool EnemyWanderer::Init(GraphicsClass * graphicsClass)
{
	return EnemyBase::Init(graphicsClass);
}

void EnemyWanderer::Update()
{
	EnemyBase::Update();
}

void EnemyWanderer::Move(float x)
{
	EnemyBase::Move(x);
}
