#include "enemywanderer.h"

EnemyWanderer::EnemyWanderer()
{
	EnemyBase::EnemyBase();
	useGravity = true;
	gravity = 5.0f;
	speed = 0.25f;
}

EnemyWanderer::EnemyWanderer(const EnemyWanderer &)
{
}

EnemyWanderer::~EnemyWanderer()
{
}

bool EnemyWanderer::Init(GraphicsClass* graphicsClass, float width, float height, float translationX, float translationY, CHAR* animationSheetName)
{
	bool toReturn = EnemyBase::Init(graphicsClass, width, height, translationX, translationY, "mob_spikyback.dds");
	if (!toReturn)
		return false;

	m_shader->AddModel(m_model);
	//Animations
	m_animation = new PlayerAnimationStates(2);
	m_shader->ImportFile(128, 128, 1024, 1024);
	m_shader->CreateNewAnimation(1, 10, 0); //IDLE
	m_shader->CreateNewAnimation(8, 5, 0); //MOVING
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

void EnemyWanderer::Update()
{
	EnemyBase::Update();
}

void EnemyWanderer::FixedUpdate()
{
	EnemyBase::FixedUpdate();

	//if (frameMovementRight != 0.0f)
	//{
	//	m_model->movingRight = frameMovementRight > 0.0f;
	//	SetNewAnimation(MOVING);
	//}
	//else
	//{
	//	SetNewAnimation(IDLE);
	//}
	
	m_model->movingRight = frameMovementRight > 0.0f;
	Move(frameMovementRight);
}

bool EnemyWanderer::TouchedPlayer(Player * player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY)
{
	if (m_destroyed)
		return false;

	if ((m_model->GetBounds().min.x < playerMaxX && playerMaxX < m_model->GetBounds().max.x) || //Enter from the left side		
		(m_model->GetBounds().max.x > playerMinX && playerMinX > m_model->GetBounds().min.x)) //Enter from the right side
	{
		if (playerMinY < m_model->GetBounds().max.y && playerMaxY > m_model->GetBounds().max.y || //Enter from the bottom
			playerMaxY > m_model->GetBounds().min.y && m_model->GetBounds().max.y > playerMinY) //Enter from the top
		{
			player->DealDamage(1);
			PlayOneShotAnimation(ATTACKING);
			return true;
		}
	}

	return false;
}

void EnemyWanderer::Move(float x)
{
	EnemyBase::Move(x);
}
