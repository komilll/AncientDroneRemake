#include "enemyarcher.h"

EnemyArcher::EnemyArcher()
{
	gravity = 10.0f;
}

EnemyArcher::EnemyArcher(const EnemyArcher &)
{
}

EnemyArcher::~EnemyArcher()
{
}

bool EnemyArcher::Init(GraphicsClass * graphicsClass, float width, float height, float translationX, float translationY, CHAR* animationSheetName)
{
	for (int i = 0; i < 5; i++)	
		m_arrow.push_back(new EnemyArcherArrow());	
	
	bool toReturn = EnemyBase::Init(graphicsClass, width, height, translationX, translationY, "mob_archer.dds");
	if (!toReturn)
		return false;

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
	arrowSpawnPoint = D3DXVECTOR2(0.25f, 0.75f);

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
		m_arrow.at(m_arrowIndex);
		m_arrowIndex++;

		//m_arrow.at(m_arrowIndex)->Init(m_graphics, 8.0f, 8.0f, 0.0f, 0.0f, "mob_archer.dds");
	}
}

void EnemyArcher::SetPlayer(Player * player_)
{
	player = player_;
}