#include "enemywanderer.h"

EnemyWanderer::EnemyWanderer()
{
	EnemyBase::EnemyBase();
	m_wander = true;
	useGravity = true;
	gravity = 5.0f;
	speed = 0.25f;
	m_hitedWall = true;
}

EnemyWanderer::EnemyWanderer(const EnemyWanderer &)
{
}

EnemyWanderer::~EnemyWanderer()
{
}

bool EnemyWanderer::Init(GraphicsClass* graphicsClass, float width, float height, float translationX, float translationY, CHAR* animationSheetName)
{
	bool toReturn = EnemyBase::Init(graphicsClass, width, height, translationX, translationY, "mob_spikyback.dds");
	if (!toReturn)
		return false;

	m_shader->AddModel(m_model);
	//Animations
	m_animation = new PlayerAnimationStates(2);
	m_shader->ImportFile(128, 128, 1024, 1024);
	m_shader->CreateNewAnimation(1, 10, 0); //IDLE
	m_shader->CreateNewAnimation(8, 5, 0); //MOVING
	m_shader->CreateNewAnimation(3, 5, 1, false); //ATTACKING

	m_animation->PrepareAnimationPose(IDLE, IDLE);
	m_animation->PrepareAnimationPose(MOVING, MOVING);
	m_animation->PrepareAnimationPose(ATTACKING, ATTACKING);
	m_animation->SetState(IDLE);
	SetNewAnimation(MOVING);

	m_wander = true;

	m_model->SetBounds(-width + 6, width - 3, -height + 2, height - 10);	

	return toReturn;
}

void EnemyWanderer::Update()
{
	EnemyBase::Update();
}

void EnemyWanderer::FixedUpdate()
{
	//EnemyBase::FixedUpdate();

	if (invincibilityTimeCurrent != 0) //Miliseconds
	{
		invincibilityTimeCurrent -= 20;
		m_shader->SetColorTint(D3DXVECTOR4(1.0f, 1.0f - (float)invincibilityTimeCurrent / (float)invincibilityTime, 1.0f - (float)invincibilityTimeCurrent / (float)invincibilityTime, 1.0f));
		if (invincibilityTimeCurrent <= 0)
		{
			invincible = false;
			invincibilityTimeCurrent = 0;
			m_shader->SetColorTint(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	if (m_destroyed)
		return;

	bool isFalling = useGravity;
	bool isGround = false;

	for (int i = 0; i < m_graphics->GetGroundModelCount(); i++)
	{
		float mMinX = m_model->GetBounds().min.x;
		float gMaxX = m_graphics->GetGroundModel(i)->GetBounds().max.x;

		if (mMinX >= gMaxX)
			continue;

		float mMaxX = m_model->GetBounds().max.x;
		float gMinX = m_graphics->GetGroundModel(i)->GetBounds().min.x;

		if (mMaxX <= gMinX)
			continue;

		float mMinY = m_model->GetBounds().min.y;
		float gMaxY = m_graphics->GetGroundModel(i)->GetBounds().max.y;

		if (mMinY > gMaxY)
			continue;

		float mMaxY = m_model->GetBounds().max.y;
		float gMinY = m_graphics->GetGroundModel(i)->GetBounds().min.y;

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
					if (gMinX - mMaxX <= 0.0f && frameMovementRight > 0.0f)
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

		if (m_wanderTimer <= 0)
		{
			if (m_wander && (groundInTheMiddle || groundFromTheBottom || groundFromTheTop))
			{
				if (mMinX <= gMaxX && gMaxX < mMaxX && speed < 0.0f)
				{
					speed = abs(speed);
					frameMovementRight = speed;
					m_wanderTimer = 100;
				}
				else if (mMaxX >= gMinX && mMinX < gMinX && mMaxX < gMaxX && speed > 0.0f)
				{
					speed = -abs(speed);
					frameMovementRight = speed;
					m_wanderTimer = 100;
				}
			}
			else if (m_wander && m_moveOnlyVertically == false)
			{
				
				if (mMaxX > gMaxX && speed > 0.0f && i + 1 < m_graphics->GetGroundModelCount() &&
					(m_graphics->GetGroundModel(i)->GetTranslation().y != m_graphics->GetGroundModel(i + 1)->GetTranslation().y ||
						gMaxX + 1 < m_graphics->GetGroundModel(i + 1)->GetBounds().min.x))
				{
					speed = -abs(speed);
					frameMovementRight = speed;
					m_wanderTimer = 100;
				}
				else if (mMinX < gMinX && speed < 0.0f && i > 0 &&
					(m_graphics->GetGroundModel(i)->GetTranslation().y != m_graphics->GetGroundModel(i - 1)->GetTranslation().y ||
						gMinX - 1 > m_graphics->GetGroundModel(i - 1)->GetBounds().max.x))
				{
					speed = abs(speed);
					frameMovementRight = speed;
					m_wanderTimer = 100;
				}
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
	m_wanderTimer -= 20;

	m_model->SetTranslation(m_model->GetTranslation().x + Forward().x * frameMovementRight, m_model->GetTranslation().y + Forward().y * frameMovementRight + frameMovementUp, m_model->GetTranslation().z);
	if (m_colliderModel)
	{
		m_colliderModel->SetTranslation(m_model->GetTranslation().x, m_model->GetTranslation().y, m_model->GetTranslation().z);
		if (m_model->UseRotation())
			m_colliderModel->SetRotation(m_model->GetRotation());
	}

	//if (frameMovementRight != 0.0f)
	//{
	//	m_model->movingRight = frameMovementRight > 0.0f;
	//	SetNewAnimation(MOVING);
	//}
	//else
	//{
	//	SetNewAnimation(IDLE);
	//}
	
	m_model->movingRight = frameMovementRight > 0.0f;
	Move(frameMovementRight);
}

bool EnemyWanderer::TouchedPlayer(Player * player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY)
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
			PlayOneShotAnimation(ATTACKING);
			return true;
		}
	}

	return false;
}

void EnemyWanderer::Move(float x)
{
	EnemyBase::Move(x);
}

void EnemyWanderer::HitedWall()
{
	//MovingObjectPrototype::HitedWall();
	speed = -speed;
}

void EnemyWanderer::ForceVerticalMovement()
{
	m_moveOnlyVertically = true;
}
