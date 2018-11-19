#include "dronespearclass.h"

DroneSpearClass::DroneSpearClass()
{
	MovingObjectPrototype::MovingObjectPrototype();

	speed = 0.5f;
}

bool DroneSpearClass::Init(GraphicsClass * graphicsClass, float width, float height, float translationX, float translationY, CHAR * animationSheetName)
{
	bool toReturn = MovingObjectPrototype::Init(graphicsClass, width, height, translationX, translationY, "spear.dds");
	if (!toReturn)
		return false;

	m_shader->AddModel(m_model);
	//Animations
	m_animation = new PlayerAnimationStates(2);
	m_shader->ImportFile(64, 64, 1024, 1024);
	m_shader->CreateNewAnimation(1, 10, 0); //IDLE
	m_shader->CreateNewAnimation(4, 5, 0, false); //ATTACKING

	m_animation->PrepareAnimationPose(IDLE, IDLE);
	m_animation->PrepareAnimationPose(ATTACKING, ATTACKING);
	m_animation->SetState(IDLE);
	SetNewAnimation(IDLE);

	m_wander = false;
	useGravity = false;

	m_model->SetBounds(-width, width, -height, height);

	return toReturn;
}

void DroneSpearClass::Update()
{
	MovingObjectPrototype::Update();
}

void DroneSpearClass::FixedUpdate()
{
	MovingObjectPrototype::FixedUpdate();
}

void DroneSpearClass::Move(float x)
{
	MovingObjectPrototype::Move(x);
}

bool DroneSpearClass::TouchedPlayer(Player * player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY)
{
	return false;
}

void DroneSpearClass::SetNewAnimation(int newState)
{
	MovingObjectPrototype::SetNewAnimation(newState);
}

void DroneSpearClass::PlayOneShotAnimation(int state, int previousState)
{
	MovingObjectPrototype::PlayOneShotAnimation(state, previousState);
}

void DroneSpearClass::HitedWall()
{
	return;
}