#include "enemyarcher.h"

EnemyArcher::EnemyArcher()
{
	gravity = 5.0f;
	m_hitedWall = false;
	speed = 0.0f;
}

EnemyArcher::EnemyArcher(const EnemyArcher &)
{
}

EnemyArcher::~EnemyArcher()
{
}

bool EnemyArcher::Init(GraphicsClass * graphicsClass, float width, float height, float translationX, float translationY, CHAR* animationSheetName)
{
	bool toReturn = EnemyBase::Init(graphicsClass, width, height, translationX, translationY, "mob_archer.dds");
	if (!toReturn)
		return false;

	for (int i = 0; i < 10; i++)
	{
		m_arrow.push_back(new EnemyArcherArrow());
		m_arrow.at(i)->Init(m_graphics, 8.0f, 8.0f, 0.0f, 0.0f, "mob_archer.dds");
	}

	m_shader->AddModel(m_model);
	//Animations
	m_animation = new PlayerAnimationStates(2);
	m_shader->ImportFile(64, 64, 1024, 1024);
	m_shader->CreateNewAnimation(1, 10, 0); //IDLE
	m_shader->CreateNewAnimation(7, 5, 0); //SHOOTING

	m_animation->PrepareAnimationPose(IDLE, IDLE);
	m_animation->PrepareAnimationPose(SHOOTING, SHOOTING);
	m_animation->SetState(IDLE);

	m_model->SetBounds(-width + 8, width - 8, -height, height);
	arrowSpawnPoint = D3DXVECTOR2(0.25f, 0.60f);	

	return toReturn;
}

void EnemyArcher::Update()
{
	EnemyBase::Update();

	if (player)
	{
		if (player->GetBounds().max.x < m_model->GetBounds().min.x)
			m_model->movingRight = false;
		else if (player->GetBounds().min.x > m_model->GetBounds().max.x)
			m_model->movingRight = true;

		lookingRight = m_model->movingRight;
	}
	
	for (int i = 0; i < m_arrow.size(); i++)
	{
		if (m_arrow.at(i)->IsActive())
			m_arrow.at(i)->Update();
	}
}

void EnemyArcher::FixedUpdate()
{
	EnemyBase::FixedUpdate();

	m_arrowCooldownCurrent += 0.02f;
	if (m_arrowCooldownCurrent >= m_arrowCooldownTime)
	{
		m_arrowCooldownCurrent = 0.0f;
		PlayOneShotAnimation(SHOOTING);

		m_arrowIndex %= m_arrow.size();
		if (m_arrow.at(m_arrowIndex)->IsActive())
		{			
			m_arrow.insert(m_arrow.begin() + m_arrowIndex, new EnemyArcherArrow());
			m_arrow.at(m_arrowIndex)->Init(m_graphics, 8.0f, 8.0f, 0.0f, 0.0f, "mob_archer.dds");
		}

		float posX = m_model->movingRight ? m_model->GetBounds().max.x : m_model->GetBounds().min.x;
		float posY = m_model->GetBounds().min.y + (m_model->GetBounds().max.y - m_model->GetBounds().min.y) * arrowSpawnPoint.y;
		m_arrow.at(m_arrowIndex)->SetDirection(m_model->movingRight, posX, posY);
		m_arrowIndex++;
	}
}

bool EnemyArcher::TouchedPlayer(Player * player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY)
{
	for (int i = 0; i < m_arrow.size(); i++)
	{
		if (m_arrow.at(i)->IsActive())
		{
			if (m_arrow.at(i)->TouchedPlayer(player, playerMinX, playerMaxX, playerMinY, playerMaxY))			
				m_arrow.at(i)->DestroyArrow();			
		}
	}
	//return EnemyBase::TouchedPlayer(player, playerMinX, playerMaxX, playerMinY, playerMaxY);
	return false;
}

void EnemyArcher::Shutdown()
{
	EnemyBase::Shutdown();
	for (int i = 0; i < m_arrow.size(); i++)
	{
		m_arrow.at(i)->Shutdown();
		delete m_arrow.at(i);
	}
	m_arrow.clear();
}

void EnemyArcher::SetPlayer(Player * player_)
{
	player = player_;
}