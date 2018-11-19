#include "dronecontroller.h"

DroneController::DroneController()
{
	MovingObjectPrototype::MovingObjectPrototype();

	speed = 0;
}

bool DroneController::Init(GraphicsClass * graphicsClass, float width, float height, float translationX, float translationY, CHAR * animationSheetName)
{
	bool toReturn = MovingObjectPrototype::Init(graphicsClass, width, height, translationX, translationY, "ancient_ball.dds");
	if (!toReturn)
		return false;

	for (int i = 0; i < 10; i++)
	{
		m_spears.push_back(new DroneSpearClass());
		m_spears.at(i)->Init(m_graphics, 8.0f, 8.0f, 0.0f, 0.0f, "mob_archer.dds");		
	}

	m_shader->AddModel(m_model);
	//Animations
	m_animation = new PlayerAnimationStates(2);
	m_shader->ImportFile(64, 64, 1024, 1024);
	m_shader->CreateNewAnimation(1, 10, 0); //IDLE
	m_shader->CreateNewAnimation(6, 5, 0, false); //ATTACKING

	m_animation->PrepareAnimationPose(IDLE, IDLE);	
	m_animation->PrepareAnimationPose(ATTACKING, ATTACKING);
	m_animation->SetState(IDLE);
	SetNewAnimation(IDLE);

	m_wander = false;
	useGravity = false;

	m_model->SetBounds(-width, width, -height, height);

	return toReturn;
}

void DroneController::Update()
{
	MovingObjectPrototype::Update();
}

void DroneController::FixedUpdate()
{
	MovingObjectPrototype::FixedUpdate();
}

void DroneController::Move(float x)
{
	MovingObjectPrototype::Move(x);
}

bool DroneController::TouchedPlayer(Player * player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY)
{
	return false;
}

void DroneController::SetNewAnimation(int newState)
{
	MovingObjectPrototype::SetNewAnimation(newState);
}

void DroneController::PlayOneShotAnimation(int state, int previousState)
{
	MovingObjectPrototype::PlayOneShotAnimation(state, previousState);
}

void DroneController::HitedWall()
{
	return;
}

bool DroneController::Attack()
{
	PlayOneShotAnimation(ATTACKING);
	//TODO Check if attack is not on cooldown
	return false;
}