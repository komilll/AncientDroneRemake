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

bool EnemyWanderer::Init(GraphicsClass* graphicsClass, float width, float height, float translationX, float translationY, CHAR* animationSheetName)
{
	return EnemyBase::Init(graphicsClass, width, height, translationX, translationY);
}

void EnemyWanderer::Update()
{
	EnemyBase::Update();
	Move(frameMovementRight);
}

void EnemyWanderer::Move(float x)
{
	EnemyBase::Move(x);
}
