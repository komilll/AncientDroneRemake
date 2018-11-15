#include "enemyarcherarrow.h"

EnemyArcherArrow::EnemyArcherArrow()
{
	MovingObjectPrototype::MovingObjectPrototype();
	useGravity = false;
}

bool EnemyArcherArrow::Init(GraphicsClass * graphicsClass, float width, float height, float translationX, float translationY, CHAR * animationSheetName)
{
	if (m_init)	
		return true;
	
	bool toReturn = MovingObjectPrototype::Init(graphicsClass, width, height, translationX, translationY, "mob_archer.dds");
	if (!toReturn)
		return false;

	m_shader->AddModel(m_model);
	//Animations
	m_animation = new PlayerAnimationStates(1);
	m_shader->ImportFile(64, 64, 1024, 1024);
	m_shader->CreateNewAnimation(1, 10, 1); //IDLE	
	m_shader->CreateNewAnimation(5, 5, 1, false); //IDLE	

	m_animation->PrepareAnimationPose(IDLE, IDLE);
	m_animation->PrepareAnimationPose(DESTROY, DESTROY);
	m_animation->SetState(IDLE);

	m_init = true;
	height = 0;
	m_model->SetBounds(-width + 1, width - 1, -height, height);	
	m_model->SetVisibility(false);

	return toReturn;
}

void EnemyArcherArrow::Update()
{
	MovingObjectPrototype::Update();
}

void EnemyArcherArrow::FixedUpdate()
{
	currentTimeToDestroy += 0.02f;
	if (currentTimeToDestroy >= timeToDestroy)
	{
		currentTimeToDestroy = 0.0f;
		DestroyArrow();
		return;
	}

	m_model->movingRight = frameMovementRight > 0;
	MovingObjectPrototype::FixedUpdate();
	Move(frameMovementRight);		
}

bool EnemyArcherArrow::IsActive()
{
	return m_isMoving;
}

void EnemyArcherArrow::SetDirection(bool movingRight, float posX, float posY)
{
	currentTimeToDestroy = 0.0f;
	m_isMoving = true;
	speed = abs(speed) * movingRight ? 1.0f : -1.0f;
	m_model->SetVisibility(true);
	SetNewAnimation(IDLE);

	if (posY != -1)
		m_model->SetTranslation(posX, posY, 0.0f);	
	else if (posX != -1)
		m_model->SetTranslation(posX, m_model->GetTranslation().y, 0.0f);
}

void EnemyArcherArrow::DestroyArrow()
{
	SetNewAnimation(DESTROY);
	m_isMoving = false;
}

void EnemyArcherArrow::HitedWall()
{
	DestroyArrow();
}