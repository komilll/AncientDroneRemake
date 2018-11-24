#include "dronespearclass.h"

DroneSpearClass::DroneSpearClass()
{
	MovingObjectPrototype::MovingObjectPrototype();

	speed = 3.0f;
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

	//m_colliderModel = new ModelClass();
	//m_colliderModel->Initialize(m_graphics->GetD3D()->GetDevice(), width, height);
	//m_colliderModel->SetTranslation(translationX, translationY, 0.0f);
	//m_colliderModel->SetVisibility(false);

	//ColorShaderClass* m_shaderColor = new ColorShaderClass();
	//if (!m_shaderColor)
	//	return false;

	//if (!m_shaderColor->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND()))
	//	return false;

	//if (!m_graphics->AddColorShader(m_shaderColor))
	//{
	//	m_shaderColor->Shutdown();
	//	delete m_shaderColor;
	//	m_shaderColor = 0;
	//}

	//m_shaderColor->AddModel(m_colliderModel);
	//m_shaderColor->SetColor(D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f));

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

void DroneSpearClass::HeightTest(float mMinX, float mMaxX, float mMinY, float mMaxY, float gMinX, float gMaxX, float gMinY, float gMaxY, ModelClass * groundModel)
{
	if (mMinX < gMaxX && gMaxX < mMaxX)// && frameMovementRight * Forward().x < 0.0f)
	{
		//m_model->SetTranslation(m_model->GetTranslation().x - (m_graphics->GetGroundModel(i)->GetBounds().max.x - m_model->GetBounds().min.x) * Forward().x,
		//	m_model->GetTranslation().y, m_model->GetTranslation().z);
		HitedWall();
	}
	else if (mMaxX > gMinX && mMinX > gMinX)//&& frameMovementRight * Forward().x > 0.0f)
	{
		//m_model->SetTranslation(m_model->GetTranslation().x + (m_graphics->GetGroundModel(i)->GetBounds().min.x - m_model->GetBounds().max.x) * Forward().x,
		//	m_model->GetTranslation().y, m_model->GetTranslation().z);
		HitedWall();
	}
}

void DroneSpearClass::Spawn()
{
	SetNewAnimation(IDLE);
	m_init = true;
	m_isMoving = true;
	m_model->SetVisibility(true);
	if (m_colliderModel)
		m_colliderModel->SetVisibility(true);
}

bool DroneSpearClass::TouchedEnemy(MovingObjectPrototype *object)
{
	if (!m_init || !m_isMoving)
		return false;

	ModelClass* enemyModel = object->GetModel();

	if ((m_model->GetBounds().min.x < enemyModel->GetBounds().max.x && enemyModel->GetBounds().max.x < m_model->GetBounds().max.x) || //Enter from the left side		
		(m_model->GetBounds().max.x > enemyModel->GetBounds().min.x && enemyModel->GetBounds().min.x > m_model->GetBounds().min.x)) //Enter from the right side
	{
		if (enemyModel->GetBounds().min.y < m_model->GetBounds().max.y && enemyModel->GetBounds().min.y > m_model->GetBounds().max.y || //Enter from the bottom
			enemyModel->GetBounds().max.y > m_model->GetBounds().min.y && m_model->GetBounds().max.y > enemyModel->GetBounds().min.y) //Enter from the top
		{
			if (object->DamageObject())
			{
				Destroy();
				return true;
			}
			else
				return false;
		}
	}

	return false;
}

void DroneSpearClass::Destroy()
{
	SetNewAnimation(DESTROY);
	m_isMoving = false;
	m_init = false;
}