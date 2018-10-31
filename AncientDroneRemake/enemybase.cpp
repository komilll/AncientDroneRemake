#include "enemybase.h"

EnemyBase::EnemyBase()
{
	timer = 0;
	lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

EnemyBase::EnemyBase(const EnemyBase &)
{
}

EnemyBase::~EnemyBase()
{
}

bool EnemyBase::Init(GraphicsClass* graphicsClass)
{
	m_graphics = graphicsClass;
	if (m_graphics == nullptr)
	{	
		return false;
	}

	m_model = new ModelClass();
	if (!m_model)
	{
		return false;
	}

	m_model->Initialize(m_graphics->GetD3D()->GetDevice(), 16, 16);
	m_graphics->AddEnemyModel(m_model);

	return true;
}

void EnemyBase::Update()
{
	float frameMovementRight = 0.0f;
	float frameMovementUp = 0.0f;

	__int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	timer += (now - lastTime);
	lastTime = now;

	if (timer >= 20.0f) //20ms = 0.02s
	{
		bool isFalling = true;
		bool isGround = false;

		for (int i = 0; i < m_graphics->GetGroundModelCount(); i++)
		{
			float groundThickness = (m_graphics->GetGroundModel(i)->GetBounds().max.y - m_graphics->GetGroundModel(i)->GetBounds().min.y) / 4;

			if (m_model->GetBounds().min.y <= m_graphics->GetGroundModel(i)->GetBounds().max.y + groundThickness)
			{
				if (m_model->GetBounds().min.x < m_graphics->GetGroundModel(i)->GetBounds().max.x
					&& m_model->GetBounds().max.x > m_graphics->GetGroundModel(i)->GetBounds().min.x)
				{
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
		}

		if (isFalling)
			frameMovementUp -= gravity;

		timer -= 20.0f;
	}

	m_model->SetTranslation(m_model->GetTranslation().x, m_model->GetTranslation().y + frameMovementUp, 0.0f);
}

void EnemyBase::Move(float x)
{
	m_model->SetTranslation(m_model->GetTranslation().x + x, m_model->GetTranslation().y, 0.0f);
}
