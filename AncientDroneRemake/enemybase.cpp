#include "enemybase.h"

EnemyBase::EnemyBase()
{
	MovingObjectPrototype::MovingObjectPrototype();
}

EnemyBase::EnemyBase(const EnemyBase &)
{
}

EnemyBase::~EnemyBase()
{
}

bool EnemyBase::Init(GraphicsClass* graphicsClass, float width, float height, float translationX, float translationY, CHAR* animationSheetName)
{
	return MovingObjectPrototype::Init(graphicsClass, width, height, translationX, translationY, animationSheetName);
}

void EnemyBase::Update()
{
	MovingObjectPrototype::Update();
}

void EnemyBase::FixedUpdate()
{
	MovingObjectPrototype::FixedUpdate();
}

void EnemyBase::Move(float x)
{
	MovingObjectPrototype::Move(x);
}

bool EnemyBase::TouchedPlayer(Player* player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY)
{
	return MovingObjectPrototype::TouchedPlayer(player, playerMinX, playerMaxX, playerMinY, playerMaxY);
}

void EnemyBase::SetNewAnimation(int newState)
{	
	MovingObjectPrototype::SetNewAnimation(newState);
}

void EnemyBase::PlayOneShotAnimation(int newState, int previousState)
{
	MovingObjectPrototype::PlayOneShotAnimation(newState, previousState);
}