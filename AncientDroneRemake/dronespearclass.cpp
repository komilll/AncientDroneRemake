#include "dronespearclass.h"

DroneSpearClass::DroneSpearClass()
{
	MovingObjectPrototype::MovingObjectPrototype();

	speed = 5.0f;
	m_wander = false;
	useGravity = false;
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
	m_shader->CreateNewAnimation(5, 4, 0, false); //DESTROY

	m_animation->PrepareAnimationPose(IDLE, IDLE);
	m_animation->PrepareAnimationPose(DESTROY, DESTROY);
	m_animation->SetState(IDLE);
	SetNewAnimation(IDLE);

	m_wander = false;
	useGravity = false;

	m_model->SetBounds(-width, width, -height, height);
	m_model->SetVisibility(false);

	return toReturn;
}

void DroneSpearClass::Update()
{
	MovingObjectPrototype::Update();
}

void DroneSpearClass::FixedUpdate()
{
	if (!m_isMoving)
		return;

	currentTimeToDestroy += 0.02f;
	if (currentTimeToDestroy >= timeToDestroy)
	{
		currentTimeToDestroy = 0.0f;
		Destroy();
		return;
	}

	//m_model->movingRight = frameMovementRight > 0;
	MovingObjectPrototype::FixedUpdate();
	//Move(frameMovementRight);
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
	Destroy();
}

void DroneSpearClass::Spawn()
{
	SetNewAnimation(IDLE);
	m_init = true;
	m_isMoving = true;
	m_model->SetVisibility(true);
}

void DroneSpearClass::Destroy()
{
	SetNewAnimation(DESTROY);
	m_isMoving = false;
	m_init = false;
}