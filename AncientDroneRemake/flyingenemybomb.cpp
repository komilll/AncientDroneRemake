#include "flyingenemybomb.h"

FlyingEnemyBomb::FlyingEnemyBomb(GraphicsClass* graphicsClass, float radius, float explosionTime, float maxScale, float gravityScale)
{
	m_graphics = 0;
	m_model = 0;

	m_isExploding = false;
	m_used = false;
	m_init = false;

	Init(graphicsClass, radius, explosionTime, maxScale, gravityScale);
}

void FlyingEnemyBomb::Update()
{
	if (!m_init)
		return;	

	//Apply gravity if hasn't started exploding yet
	if (!m_isExploding)
	{
		m_model->SetTranslation(m_model->GetTranslation().x, m_model->GetTranslation().y - m_gravityScale, 0.0f);

		for (int i = 0; i < m_graphics->GetGroundModelCount(); i++)
		{
			//Checking vertical
			if (m_model->GetBounds().min.y < m_graphics->GetGroundModel(i)->GetBounds().max.y &&
				m_model->GetBounds().max.y > m_graphics->GetGroundModel(i)->GetBounds().max.y &&
				m_model->GetBounds().min.y > m_graphics->GetGroundModel(i)->GetBounds().min.y)
			{
				//Checking horizontal
				if (m_model->GetBounds().max.x > m_graphics->GetGroundModel(i)->GetBounds().min.x &&
					m_model->GetBounds().min.x < m_graphics->GetGroundModel(i)->GetBounds().max.x)
				{
					float translationY = m_graphics->GetGroundModel(i)->GetBounds().max.y - m_model->GetBounds().min.y;
					m_model->SetTranslation(m_model->GetTranslation().x, m_model->GetTranslation().y + translationY, 0.0f);					
					m_isExploding = true;
					break;
				}
			}
		}
		return;
	}

	//Update explosion after hitted ground
	if (!m_used)
	{
		for (int i = 0; i < m_graphics->GetGroundModelCount(); i++)
		{
			//Checking vertical
			if (m_model->GetBounds().min.y < m_graphics->GetGroundModel(i)->GetBounds().max.y &&
				m_model->GetBounds().max.y > m_graphics->GetGroundModel(i)->GetBounds().max.y &&
				m_model->GetBounds().min.y > m_graphics->GetGroundModel(i)->GetBounds().min.y)
			{
				//Checking horizontal
				if (m_model->GetBounds().max.x > m_graphics->GetGroundModel(i)->GetBounds().min.x &&
					m_model->GetBounds().min.x < m_graphics->GetGroundModel(i)->GetBounds().max.x)
				{
					float translationY = m_graphics->GetGroundModel(i)->GetBounds().max.y - m_model->GetBounds().min.y;
					m_model->SetTranslation(m_model->GetTranslation().x, m_model->GetTranslation().y + translationY, 0.0f);
					break;
				}
			}
		}

		m_currentScale += 0.025f;
		if (m_currentScale > m_explosionMaxScale)
		{
			m_currentScale = m_explosionMaxScale;
		}
		m_model->SetScale(m_currentScale, m_currentScale, 1.0f);
		return;
	}
}

ModelClass * FlyingEnemyBomb::GetModel()
{
	return m_model;
}

void FlyingEnemyBomb::Init(GraphicsClass* graphicsClass, float radius, float explosionTime, float maxScale, float gravityScale)
{
	if ( (m_graphics = graphicsClass) == nullptr)
		return;

	m_model = new ModelClass();
	if (!m_model)
		return;

	if (!m_model->Initialize(m_graphics->GetD3D()->GetDevice(), radius, radius))
		return;

	m_graphics->AddBombModel(m_model);

	m_explosionRadius = radius;
	m_explosionTime = explosionTime;
	m_explosionMinScale = 1.0f;
	m_explosionMaxScale = maxScale;
	m_gravityScale = gravityScale;

	m_currentScale = m_explosionMinScale;

	m_init = true;
}

bool FlyingEnemyBomb::TouchedPlayer(Player* player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY)
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