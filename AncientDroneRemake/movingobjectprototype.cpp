#include "movingobjectprototype.h"

MovingObjectPrototype::MovingObjectPrototype()
{
	m_shader = 0;
	timer = 0;
	m_colliderModel = 0;
	lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

MovingObjectPrototype::MovingObjectPrototype(const MovingObjectPrototype &)
{
}

MovingObjectPrototype::~MovingObjectPrototype()
{
}

bool MovingObjectPrototype::Init(GraphicsClass * graphicsClass, float width, float height, float translationX, float translationY, CHAR * animationSheetName)
{
	m_graphics = graphicsClass;
	if (m_graphics == nullptr)
		return false;

	m_model = new ModelClass();
	if (!m_model)
		return false;

	if (animationSheetName != "")
	{
		m_shader = new TextureShaderClass();
		if (!m_shader)
			return false;

		if (!m_shader->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND(), animationSheetName))
			return false;

		if (!m_graphics->AddTextureShader(m_shader))
		{
			m_shader->Shutdown();
			delete m_shader;
			m_shader = 0;
		}
	}

	m_model->Initialize(m_graphics->GetD3D()->GetDevice(), width, height);
	m_model->SetTranslation(translationX, translationY, 0.0f);

	if (m_shader)
		m_shader->SetAnimationObject(this);

	return true;
}

void MovingObjectPrototype::Update()
{
	if (m_destroyed)
		return;

	frameMovementRight = speed;
	frameMovementUp = 0.0f;	

	__int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	timer += (now - lastTime);
	lastTime = now;

	if (timer >= 20.0f) //20ms = 0.02s
	{
		FixedUpdate();
	}
}

void MovingObjectPrototype::FixedUpdate()
{
	if (m_destroyed)
		return;

	bool isFalling = useGravity;
	bool isGround = false;

	for (int i = 0; i < m_graphics->GetGroundModelCount(); i++)
	{
		float mMaxY = m_model->GetBounds().max.y;
		float mMinY = m_model->GetBounds().min.y;
		float gMaxY = m_graphics->GetGroundModel(i)->GetBounds().max.y;
		float gMinY = m_graphics->GetGroundModel(i)->GetBounds().min.y;

		float mMaxX = m_model->GetBounds().max.x;
		float mMinX = m_model->GetBounds().min.x;
		float gMaxX = m_graphics->GetGroundModel(i)->GetBounds().max.x;
		float gMinX = m_graphics->GetGroundModel(i)->GetBounds().min.x;

		float groundThickness = (gMaxY - gMinY) / 4;
		//Ground in the middle
		bool groundInTheMiddle = (mMaxY > gMaxY && mMinY < gMinY);
		//Ground from bottom
		bool groundFromTheBottom = (mMaxY > gMaxY && mMinY > gMinY && mMinY < gMaxY);
		//Ground from top
		bool groundFromTheTop = (mMaxY < gMaxY && mMinY < gMinY && mMaxY > gMinY);

		if (mMinY <= gMaxY)		
		{
			if (mMinX < gMaxX && mMaxX > gMinX)
			{
				if (useGravity)
					frameMovementUp = (gMaxY - mMinY);
				isFalling = false;
				isGround = true;
			}

			if (useGravity && (groundInTheMiddle || groundFromTheBottom || groundFromTheTop) && speed != 0.0f)
			{
				if (mMaxX < gMaxX)
				{
					if (gMinX - mMaxX  <= 0.0f && frameMovementRight > 0.0f)
						frameMovementRight = gMinX - mMaxX;
				}
				else if (mMinX > gMinX)
				{
					float temp_ = -(mMinX - gMaxX);

					if (temp_ >= 0.0f && frameMovementRight < 0.0f)
						frameMovementRight = temp_;
				}
			}
		}


		if (m_wander && (groundInTheMiddle || groundFromTheBottom || groundFromTheTop))
		{
			if (mMinX <= gMaxX && gMaxX < mMaxX && speed < 0.0f)
			{
				speed = abs(speed);
				frameMovementRight = speed;
			}
			else if (mMaxX >= gMinX && mMinX < gMinX && mMaxX < gMaxX && speed > 0.0f)
			{
				speed = -abs(speed);
				frameMovementRight = speed;
			}
		}		
		
		bool heightTest = (gMaxY > mMaxY && mMinY > gMinY) || (mMaxY > gMaxY && mMinY < gMinY) || (mMaxY > gMaxY && mMinY > gMinY && mMinY < gMaxY) || 
			(mMaxY < gMaxY && mMinY < gMinY && mMaxY > gMinY);

		//if (heightTest && m_hitedWall)
		if (isGround)
		{
			HeightTest(mMinX, mMaxX, mMinY, mMaxY, gMinX, gMaxX, gMinY, gMaxY, m_graphics->GetGroundModel(i), heightTest);
		}
	}

	if (isFalling)
		frameMovementUp -= gravity;

	timer -= 20.0f;

	m_model->SetTranslation(m_model->GetTranslation().x + Forward().x * frameMovementRight, m_model->GetTranslation().y + Forward().y * frameMovementRight + frameMovementUp, m_model->GetTranslation().z);
	if (m_colliderModel)
	{
		m_colliderModel->SetTranslation(m_model->GetTranslation().x, m_model->GetTranslation().y, m_model->GetTranslation().z);
		if (m_model->UseRotation())
			m_colliderModel->SetRotation(m_model->GetRotation());
	}
}

void MovingObjectPrototype::Move(float x)
{
	m_model->SetTranslation(m_model->GetTranslation().x + x, m_model->GetTranslation().y, 0.0f);
}

bool MovingObjectPrototype::TouchedPlayer(Player * player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY)
{
	if (m_destroyed)
		return false;

	if ((m_model->GetBounds().min.x < playerMaxX && playerMaxX < m_model->GetBounds().max.x) || //Enter from the left side		
		(m_model->GetBounds().max.x > playerMinX && playerMinX > m_model->GetBounds().min.x)) //Enter from the right side
	{
		if (playerMinY < m_model->GetBounds().max.y && playerMaxY > m_model->GetBounds().max.y || //Enter from the bottom
			playerMaxY > m_model->GetBounds().min.y && m_model->GetBounds().max.y > playerMinY) //Enter from the top
		{
			player->DealDamage(1, GetModel()->GetTranslation());
			return true;
		}
	}

	return false;
}

void MovingObjectPrototype::SetNewAnimation(int newState)
{
	if (newState == m_animation->GetCurrentState())
		return;

	m_animation->SetState(newState);
	m_graphics->SetPlayerAnimation(m_animation->GetCurrentState(), m_shader);
}

void MovingObjectPrototype::PlayOneShotAnimation(int newState, int previousState)
{
	if (newState == m_animation->GetCurrentState())
		return;

	m_animation->SetState(newState);
	m_graphics->SetPlayerAnimationOneShot(m_animation->GetCurrentState(), m_shader, previousState);
}

void MovingObjectPrototype::HitedWall()
{

}

void MovingObjectPrototype::HeightTest(float mMinX, float mMaxX, float mMinY, float mMaxY, float gMinX, float gMaxX, float gMinY, float gMaxY, ModelClass* groundModel, bool heightTestPassed)
{
	bool groundInTheMiddle = (m_model->GetBounds().max.y > groundModel->GetBounds().max.y &&
		m_model->GetBounds().min.y < groundModel->GetBounds().min.y &&
		m_model->GetBounds().max.y > groundModel->GetBounds().min.y)
		||
		(m_model->GetBounds().max.y < groundModel->GetBounds().max.y &&
			m_model->GetBounds().min.y >= groundModel->GetBounds().min.y &&
			m_model->GetBounds().max.y > groundModel->GetBounds().min.y);

	if (!groundInTheMiddle)
		return;

	//if (gMaxX < mMinX && gMaxX < mMaxX && gMinX < mMaxX)
	if (gMaxX > mMaxX && mMaxX > gMinX && mMinX < gMinX)
	{
		//m_model->SetTranslation(m_model->GetTranslation().x - (groundModel->GetBounds().max.x - m_model->GetBounds().min.x) * Forward().x,
		//	m_model->GetTranslation().y, m_model->GetTranslation().z);
		//if (frameMovementRight * Forward().x < 0.0f)
		if (frameMovementRight > 0.0f)
			HitedWall();
	}
	//else if (mMaxX > gMinX && mMinX < gMinX && mMaxX < gMaxX)
	else if (gMinX < mMinX && mMinX < gMaxX && mMaxX > gMaxX)
	{
		//m_model->SetTranslation(m_model->GetTranslation().x + (groundModel->GetBounds().min.x - m_model->GetBounds().max.x) * Forward().x,
		//	m_model->GetTranslation().y, m_model->GetTranslation().z);
		if (frameMovementRight < 0.0f)
			HitedWall();
	}
}

bool MovingObjectPrototype::DamageObject()
{
	if (m_destroyed == true || !m_destroyable)
		return false;

	if (--m_health <= 0)
	{
		m_destroyed = true;
		DestroyObject();
	}

	return true;
}

bool MovingObjectPrototype::DamageObject(int dmg)
{
	if (m_destroyed == true || !m_destroyable)
		return false;

	m_health -= dmg;
	if (m_health <= 0)
	{
		m_destroyed = true;
		DestroyObject();
	}

	return true;
}

void MovingObjectPrototype::Shutdown()
{
	m_graphics->RemoveTextureShader(m_shader);
	if (m_model)
	{
		m_model->Shutdown();
		delete m_model;
		m_model = 0;
	}
	if (m_animation)
	{
		delete m_animation;
		m_animation = 0;
	}
	if (m_shader)
	{
		m_shader->Shutdown();
		delete m_shader;
		m_shader = 0;
	}
}

void MovingObjectPrototype::DestroyObject()
{
	m_model->SetVisibility(false);
}

ModelClass* MovingObjectPrototype::GetModel()
{
	return m_model;
}

ModelClass * MovingObjectPrototype::GetModelCollider()
{
	return m_colliderModel;
}

D3DXVECTOR2 MovingObjectPrototype::Position()
{
	float posX = GetModel()->GetBounds().min.x +
		(GetModel()->GetBounds().max.x - GetModel()->GetBounds().min.x) / 2.0f;
	float posY = GetModel()->GetBounds().min.y +
		(GetModel()->GetBounds().max.y - GetModel()->GetBounds().min.y) / 2.0f;

	return D3DXVECTOR2(posX, posY);
}

float MovingObjectPrototype::Lerp(float a, float b, float val)
{
	return (a * (1.0 - val)) + (b * val);
}

D3DXVECTOR2 MovingObjectPrototype::Forward()
{
	//float rotZ = m_model->GetRotation();
	//float xForward = 0.0f;
	//float yForward = 0.0f;

	//if (rotZ >= 0 && rotZ <= PI / 4.0f)
	//{
	//	xForward = Lerp(1.0f, sqrt(2.0f) / 2.0f, rotZ / (PI / 4.0f));
	//	yForward = Lerp(0, sqrt(2.0f) / 2.0f, rotZ / (PI / 4.0f));
	//}
	//else if (rotZ >= PI / 4.0f && rotZ <= PI / 2.0f) 
	//{
	//	xForward = Lerp(sqrt(2.0f) / 2.0f, 0.0f, (rotZ - PI / 4.0f) / (PI / 4.0f));
	//	yForward = Lerp(sqrt(2.0f) / 2.0f, 1.0f, (rotZ - PI / 4.0f) / (PI / 4.0f));
	//}
	//else if (rotZ >= -PI / 4.0f && rotZ <= 0.0f)
	//{
	//	xForward = Lerp(0.0f, -sqrt(2.0f) / 2.0f, rotZ / (-PI / 4.0f));
	//	yForward = Lerp(1.0f, sqrt(2.0f) / 2.0f, rotZ / (-PI / 4.0f));
	//}
	return D3DXVECTOR2(cos(m_model->GetRotation()), sin(m_model->GetRotation()));
}