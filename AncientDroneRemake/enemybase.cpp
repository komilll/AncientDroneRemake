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

	if (invincibilityTimeCurrent != 0) //Miliseconds
	{
		invincibilityTimeCurrent -= 20;
		m_shader->SetColorTint(D3DXVECTOR4(1.0f, 1.0f - (float)invincibilityTimeCurrent / (float)invincibilityTime, 1.0f - (float)invincibilityTimeCurrent / (float)invincibilityTime, 1.0f));
		if (invincibilityTimeCurrent <= 0)
		{
			invincible = false;
			invincibilityTimeCurrent = 0;
			m_shader->SetColorTint(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

void EnemyBase::Move(float x)
{
	MovingObjectPrototype::Move(x);
}

bool EnemyBase::TouchedPlayer(Player* player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY)
{
	return MovingObjectPrototype::TouchedPlayer(player, playerMinX, playerMaxX, playerMinY, playerMaxY);
}

bool EnemyBase::DamageObject()
{
	if (invincible)
		return false;

	if (!MovingObjectPrototype::DamageObject())
		return false;

	invincible = true;
	invincibilityTimeCurrent = invincibilityTime;

	m_shader->SetColorTint(D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));
}

void EnemyBase::SetNewAnimation(int newState)
{	
	MovingObjectPrototype::SetNewAnimation(newState);
}

void EnemyBase::PlayOneShotAnimation(int newState, int previousState)
{
	MovingObjectPrototype::PlayOneShotAnimation(newState, previousState);
}

void EnemyBase::RestartEnemy()
{
	m_health = m_maxHealth;
	m_model->SetVisibility(true);
	m_destroyed = false;
}