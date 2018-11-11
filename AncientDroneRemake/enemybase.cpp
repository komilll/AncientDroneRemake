#include "enemybase.h"

EnemyBase::EnemyBase()
{
	m_shader = 0;
	timer = 0;
	lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

EnemyBase::EnemyBase(const EnemyBase &)
{
}

EnemyBase::~EnemyBase()
{
}

bool EnemyBase::Init(GraphicsClass* graphicsClass, float width, float height, float translationX, float translationY, CHAR* animationSheetName)
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
	m_graphics->AddEnemyModel(m_model);

	return true;
}

void EnemyBase::Update()
{
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

void EnemyBase::FixedUpdate()
{
	bool isFalling = useGravity;
	bool isGround = false;

	for (int i = 0; i < m_graphics->GetGroundModelCount(); i++)
	{
		float groundThickness = (m_graphics->GetGroundModel(i)->GetBounds().max.y - m_graphics->GetGroundModel(i)->GetBounds().min.y) / 4;

		if (m_model->GetBounds().min.y <= m_graphics->GetGroundModel(i)->GetBounds().max.y + groundThickness)
		{
			if (m_model->GetBounds().min.x < m_graphics->GetGroundModel(i)->GetBounds().max.x
				&& m_model->GetBounds().max.x > m_graphics->GetGroundModel(i)->GetBounds().min.x)
			{
				if (useGravity)
					frameMovementUp = (m_graphics->GetGroundModel(i)->GetBounds().max.y - m_model->GetBounds().min.y);
				isFalling = false;
				isGround = true;
			}
			else if (m_model->GetBounds().max.x < m_graphics->GetGroundModel(i)->GetBounds().max.x)
			{
				if (m_graphics->GetGroundModel(i)->GetBounds().min.x - m_model->GetBounds().max.x <= 0.0f && frameMovementRight > 0.0f)
					frameMovementRight = m_graphics->GetGroundModel(i)->GetBounds().min.x - m_model->GetBounds().max.x;
			}
			else if (m_model->GetBounds().min.x > m_graphics->GetGroundModel(i)->GetBounds().min.x)
			{
				float temp_ = -(m_model->GetBounds().min.x - m_graphics->GetGroundModel(i)->GetBounds().max.x);

				if (temp_ >= 0.0f && frameMovementRight < 0.0f)
					frameMovementRight = temp_;
			}
		}

		//Ground in the middle
		if ((m_model->GetBounds().max.y > m_graphics->GetGroundModel(i)->GetBounds().max.y &&
			m_model->GetBounds().min.y < m_graphics->GetGroundModel(i)->GetBounds().min.y) ||
			//Ground from bottom
			(m_model->GetBounds().max.y > m_graphics->GetGroundModel(i)->GetBounds().max.y &&
				m_model->GetBounds().min.y > m_graphics->GetGroundModel(i)->GetBounds().min.y &&
				m_model->GetBounds().min.y < m_graphics->GetGroundModel(i)->GetBounds().max.y) ||
			//Ground from top
				(m_model->GetBounds().max.y < m_graphics->GetGroundModel(i)->GetBounds().max.y &&
					m_model->GetBounds().min.y < m_graphics->GetGroundModel(i)->GetBounds().min.y &&
					m_model->GetBounds().max.y > m_graphics->GetGroundModel(i)->GetBounds().min.y))
		{
			if (m_model->GetBounds().min.x < m_graphics->GetGroundModel(i)->GetBounds().max.x &&
				m_graphics->GetGroundModel(i)->GetBounds().max.x < m_model->GetBounds().max.x &&
				frameMovementRight < 0.0f)
			{
				speed = abs(speed);
				frameMovementRight = speed;
			}
			else if (m_model->GetBounds().max.x > m_graphics->GetGroundModel(i)->GetBounds().min.x &&
				m_graphics->GetGroundModel(i)->GetBounds().min.x > m_model->GetBounds().min.x &&
				frameMovementRight > 0.0f)
			{
				speed = -abs(speed);
				frameMovementRight = speed;
			}
		}
	}

	if (isFalling)
		frameMovementUp -= gravity;

	timer -= 20.0f;

	m_model->SetTranslation(m_model->GetTranslation().x, m_model->GetTranslation().y + frameMovementUp, 0.0f);
	//Move(frameMovementRight);
}

void EnemyBase::Move(float x)
{
	m_model->SetTranslation(m_model->GetTranslation().x + x, m_model->GetTranslation().y, 0.0f);
}

bool EnemyBase::TouchedPlayer(Player* player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY)
{
	if ((m_model->GetBounds().min.x < playerMaxX && playerMaxX < m_model->GetBounds().max.x) || //Enter from the left side		
		(m_model->GetBounds().max.x > playerMinX && playerMinX > m_model->GetBounds().min.x)) //Enter from the right side
	{
		if (playerMinY < m_model->GetBounds().max.y && playerMaxY > m_model->GetBounds().max.y || //Enter from the bottom
			playerMaxY > m_model->GetBounds().min.y && m_model->GetBounds().max.y > playerMinY) //Enter from the top

		player->DealDamage(1);
		return true;
	}

	return false;
}