#include "enemyarcherarrow.h"

EnemyArcherArrow::EnemyArcherArrow()
{
	MovingObjectPrototype::MovingObjectPrototype();
	useGravity = false;
}

bool EnemyArcherArrow::Init(GraphicsClass * graphicsClass, float width, float height, float translationX, float translationY, CHAR * animationSheetName)
{
	bool toReturn = MovingObjectPrototype::Init(graphicsClass, width, height, translationX, translationY, "mob_archer.dds");
	if (!toReturn)
		return false;

	m_shader->AddModel(m_model);
	//Animations
	m_animation = new PlayerAnimationStates(1);
	m_shader->ImportFile(64, 64, 1024, 1024);
	m_shader->CreateNewAnimation(1, 10, 0); //IDLE	

	m_animation->PrepareAnimationPose(IDLE, IDLE);
	m_animation->SetState(IDLE);

	return toReturn;
}

void EnemyArcherArrow::Update()
{
	MovingObjectPrototype::Update();
}

void EnemyArcherArrow::FixedUpdate()
{
	MovingObjectPrototype::FixedUpdate();
}