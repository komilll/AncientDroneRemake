#include "player.h"

Player::Player()
{
	m_d3d = 0;
	m_input = 0;
	m_playerModel = 0;
	timer = 0;
	lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	m_playerAnimation = 0;
}

Player::Player(InputClass * inputClass, D3DClass *d3d, GraphicsClass *graphicsClass)
{
	m_input = inputClass;
	m_d3d = d3d;
	m_graphics = graphicsClass;
}

Player::Player(const Player &)
{

}

Player::~Player()
{

}

bool Player::Initialize()
{
	//gravity = 0.0f;

	m_playerModel = new ModelClass();
	if (!m_playerModel)
		return false;

	if (m_d3d == nullptr)
		return false;

	if (!m_playerModel->Initialize(m_d3d->GetDevice(), 12, 12))
		return false;

	if (m_graphics == nullptr)
		return false;

	m_playerAnimation = new PlayerAnimationStates(4);
	if (m_playerAnimation == nullptr)
		return false;

	m_shaderClass = new TextureShaderClass();
	if (m_shaderClass == nullptr)
		return false;

	if (!m_shaderClass->Initialize(m_d3d->GetDevice(), *m_graphics->GetHWND(), "player.dds"))
		return false;

	m_graphics->AddTextureShader(m_shaderClass);	
//PREPARE ANIMATIONS
	m_shaderClass->ImportFile(64, 64, 1024, 1024);
	m_shaderClass->CreateNewAnimation(4, 10, 0); //IDLE
	m_shaderClass->CreateNewAnimation(6, 7, 1); //MOVING
	m_shaderClass->CreateNewAnimation(7, 5, 3); //COMMAND
	m_shaderClass->CreateNewAnimation(3, 5, 4, false); //FALLING

	m_playerAnimation->PrepareAnimationPose(IDLE, IDLE);
	m_playerAnimation->PrepareAnimationPose(MOVING, MOVING);
	m_playerAnimation->PrepareAnimationPose(COMMAND, COMMAND);
	m_playerAnimation->PrepareAnimationPose(FALLING, FALLING);
	m_playerAnimation->SetState(IDLE);
///////////////////
	m_playerModel->SetBounds(-4, 4, m_playerModel->GetBounds().min.y, m_playerModel->GetBounds().max.y - 3);

	m_graphics->SetPlayerModel(m_playerModel);
	//movementUp += 10.0f;
	//movementRight -= 60.0f;
}

bool Player::Initialize(InputClass * inputClass)
{
	m_input = inputClass;
	return Initialize();
}

void Player::Update()
{	
	float frameMovementRight = 0.0f;
	float frameMovementUp = 0.0f;
	movementDirection = 0.0f;
	StatePlayer newState = IDLE;

	if (m_input->IsKeyDown(btn_moveRight))
	{
		movementDirection = 1.0f;
	}
	if (m_input->IsKeyDown(btn_moveLeft))
	{
		movementDirection = -1.0f;
	}
	if (movementDirection > 0.0f)
	{
		m_playerModel->movingRight = true;
		newState = MOVING;
		idleTime = 0;
	}
	else if (movementDirection < 0.0f)
	{
		m_playerModel->movingRight = false;
		newState = MOVING;
		idleTime = 0;
	}
	else
	{
		idleTime++;
		if (idleTime >= timeToSetIdleState)
			newState = IDLE;
	}

	if (m_input->IsKeyDown(btn_jump))
	{
		if ((isGround || canDoubleJump) && holdingJumpButton == false)
		{
			if (!isGround)
				canDoubleJump = false;
			isGround = false;
			currentJumpTicks = jumpTicks;
		}
		holdingJumpButton = true;
	}
	else
	{
		holdingJumpButton = false;
	}
	
	__int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	timer += (now - lastTime);

	if (invincibilityTimeCurrent != 0) //Miliseconds
	{
		invincibilityTimeCurrent -= (now - lastTime);
		if (invincibilityTimeCurrent <= 0)
		{
			invincible = false;
			invincibilityTimeCurrent = 0;
			m_shaderClass->SetColorTint(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	lastTime = now;

	if (timer >= 20.0f) //20ms = 0.02s
	{
		bool isFalling = true;
		isGround = false;

		if (currentJumpTicks > 0)
		{
			isFalling = false;

			frameMovementUp += jumpTickHeight;
			currentJumpTicks--;
		}
		/****************************************************************/
		frameMovementRight = movementDirection * movementSpeed + scheduledMovementX;

		for (int i = 0; i < m_graphics->GetGroundModelCount(); i++)
		{
			bool inBounds = (m_playerModel->GetBounds().min.x < m_graphics->GetGroundModel(i)->GetBounds().max.x
				&& m_playerModel->GetBounds().max.x > m_graphics->GetGroundModel(i)->GetBounds().min.x);
			bool inBoundsNextFrame = (m_playerModel->GetBounds().min.x + frameMovementRight < m_graphics->GetGroundModel(i)->GetBounds().max.x
				&& m_playerModel->GetBounds().max.x + frameMovementRight > m_graphics->GetGroundModel(i)->GetBounds().min.x);
			//Ground in the middle
			bool groundInTheMiddle = (m_playerModel->GetBounds().max.y > m_graphics->GetGroundModel(i)->GetBounds().max.y &&
				m_playerModel->GetBounds().min.y < m_graphics->GetGroundModel(i)->GetBounds().min.y &&
				m_playerModel->GetBounds().max.y > m_graphics->GetGroundModel(i)->GetBounds().min.y)
				||
				(m_playerModel->GetBounds().max.y < m_graphics->GetGroundModel(i)->GetBounds().max.y &&
					m_playerModel->GetBounds().min.y >= m_graphics->GetGroundModel(i)->GetBounds().min.y &&
					m_playerModel->GetBounds().max.y > m_graphics->GetGroundModel(i)->GetBounds().min.y);
			//Ground from bottom
			bool groundFromTheBottom = (m_playerModel->GetBounds().max.y > m_graphics->GetGroundModel(i)->GetBounds().max.y &&
				m_playerModel->GetBounds().min.y > m_graphics->GetGroundModel(i)->GetBounds().min.y &&
				m_playerModel->GetBounds().min.y < m_graphics->GetGroundModel(i)->GetBounds().max.y);
			//Ground from top
			bool groundFromTheTop = (m_playerModel->GetBounds().max.y < m_graphics->GetGroundModel(i)->GetBounds().max.y &&
				m_playerModel->GetBounds().min.y < m_graphics->GetGroundModel(i)->GetBounds().min.y &&
				m_playerModel->GetBounds().max.y > m_graphics->GetGroundModel(i)->GetBounds().min.y);

			//Move right or left based on current button being held
			if (frameMovementRight == 0.0f)
			{
				//No movement is involved
			}
			else if (frameMovementRight > 0.0f) //Move right
			{
				if ((groundInTheMiddle || groundFromTheBottom) && inBounds)
				{
					//float newMovement = m_playerModel->GetBounds().max.x - m_graphics->GetGroundModel(i)->GetBounds().min.x;
					//if (newMovement < frameMovementRight)
					//	frameMovementRight = newMovement;
					frameMovementRight = -(m_playerModel->GetBounds().max.x + frameMovementRight - m_graphics->GetGroundModel(i)->GetBounds().min.x);
					break;
				}
				else if ((groundInTheMiddle || groundFromTheBottom) && inBoundsNextFrame)
				{
					frameMovementRight = -(m_playerModel->GetBounds().max.x - m_graphics->GetGroundModel(i)->GetBounds().min.x);
					break;
				}
			}
			else if (frameMovementRight < 0.0f) //Move left
			{
				if ((groundInTheMiddle || groundFromTheBottom) && inBounds)
				{
					//float newMovement = m_playerModel->GetBounds().min.x - m_graphics->GetGroundModel(i)->GetBounds().max.x;
					//if (newMovement > frameMovementRight)
					//	frameMovementRight = newMovement;
					frameMovementRight = -(m_playerModel->GetBounds().min.x + frameMovementRight - m_graphics->GetGroundModel(i)->GetBounds().max.x);
					break;
				}
				else if ((groundInTheMiddle || groundFromTheBottom) && inBoundsNextFrame)
				{
					frameMovementRight = -(m_playerModel->GetBounds().min.x - m_graphics->GetGroundModel(i)->GetBounds().max.x);
					break;
				}
			}			
		}
		if (frameMovementRight - movementDirection * movementSpeed != scheduledMovementX)
		{
			invincibilityTimeCurrent = invincibilityTimeLong;
		}
		scheduledMovementX = 0;

		/****************************************************************/
		for (int i = 0; i < m_graphics->GetGroundModelCount(); i++)
		{
			//float groundThickness = (m_graphics->GetGroundModel(i)->GetBounds().max.y - m_graphics->GetGroundModel(i)->GetBounds().min.y) / 4;
			//Check if player is inside environment #1 - in the middle
			bool inMiddle = (m_playerModel->GetBounds().max.y > m_graphics->GetGroundModel(i)->GetBounds().max.y &&
				m_playerModel->GetBounds().min.y < m_graphics->GetGroundModel(i)->GetBounds().min.y);
			//Check if player is inside environment #2 - at the top
			bool atTop = (m_playerModel->GetBounds().max.y > m_graphics->GetGroundModel(i)->GetBounds().min.y &&
				m_playerModel->GetBounds().max.y < m_graphics->GetGroundModel(i)->GetBounds().max.y &&
				m_playerModel->GetBounds().min.y < m_graphics->GetGroundModel(i)->GetBounds().min.y);
				//Check if player is in horizontal bounds of environment
			bool inBounds = (m_playerModel->GetBounds().min.x < m_graphics->GetGroundModel(i)->GetBounds().max.x
				&& m_playerModel->GetBounds().max.x > m_graphics->GetGroundModel(i)->GetBounds().min.x);

			if (frameMovementUp > 0.0f && (inMiddle || atTop) && inBounds) //Check if player is moving up
			{
				isFalling = true;
				currentJumpTicks = 0;
				frameMovementUp = -(m_playerModel->GetBounds().max.y - m_graphics->GetGroundModel(i)->GetBounds().min.y) - 0.1f;
			}
			else if (m_playerModel->GetBounds().min.y < m_graphics->GetGroundModel(i)->GetBounds().max.y)
			{
				//float playerMin_ = m_playerModel->GetBounds().min.y - groundThickness;
				//float groundMax_ = m_graphics->GetGroundModel()->GetBounds().max.y;

				//float playerMinX = m_playerModel->GetBounds().min.x;
				//float groundMaxX = m_graphics->GetGroundModel()->GetBounds().max.x;

				//frameMovementUp += (groundMax_ - playerMin_);

				if (m_playerModel->GetBounds().max.y > m_graphics->GetGroundModel(i)->GetBounds().min.y)
				{					
					if (m_playerModel->GetBounds().min.x < m_graphics->GetGroundModel(i)->GetBounds().max.x
						&& m_playerModel->GetBounds().max.x > m_graphics->GetGroundModel(i)->GetBounds().min.x)
					{
						isFalling = false;
						isGround = true;
						canDoubleJump = true;
					}
					else if (m_playerModel->GetBounds().max.x < m_graphics->GetGroundModel(i)->GetBounds().max.x && (isFalling || frameMovementUp != 0))
					{
						float temp_ = m_graphics->GetGroundModel(i)->GetBounds().min.x - m_playerModel->GetBounds().max.x;
						if (temp_ <= 0.0f && frameMovementRight > 0.0f)
							frameMovementRight = temp_;
					}
					else if (m_playerModel->GetBounds().min.x > m_graphics->GetGroundModel(i)->GetBounds().min.x && (isFalling || frameMovementUp != 0))
					{
						float temp_ = -(m_playerModel->GetBounds().min.x - m_graphics->GetGroundModel(i)->GetBounds().max.x);
						bool canContinue = false;
						//Do neighbour ground test
						if (i == m_graphics->GetGroundModelCount() - 1)
						{
							//Continue
							canContinue = true;
						}
						else if (m_graphics->GetGroundModelCount() - 1 > i && m_graphics->GetGroundModel(i + 1)->GetTranslation().y != m_graphics->GetGroundModel(i)->GetTranslation().y)
						{
							//Continue
							canContinue = true;
						}
						else if (m_graphics->GetGroundModelCount() - 1 > i && m_graphics->GetGroundModel(i + 1)->GetTranslation().y == m_graphics->GetGroundModel(i)->GetTranslation().y &&
							m_graphics->GetGroundModel(i + 1)->GetBounds().min.x + 1.0f > m_graphics->GetGroundModel(i)->GetBounds().max.x)
						{
							//STOP
							canContinue = false;
						}

						if (temp_ >= 0.0f && frameMovementRight < 0.0f && canContinue)
							frameMovementRight = temp_;
					}
				}
			}

			//Ground from bottom
			bool groundFromTheBottom = (m_playerModel->GetBounds().max.y > m_graphics->GetGroundModel(i)->GetBounds().max.y &&
				m_playerModel->GetBounds().min.y > m_graphics->GetGroundModel(i)->GetBounds().min.y &&
				m_playerModel->GetBounds().min.y <= m_graphics->GetGroundModel(i)->GetBounds().max.y);
			if (groundFromTheBottom && inBounds && frameMovementUp <= 0.0f)
			{
				isFalling = false;
				isGround = true;
				canDoubleJump = true;
			}
		}

		if (isFalling)
		{
			if (holdingJumpButton)
				frameMovementUp -= gravitySlow;
			else
				frameMovementUp -= gravitySlow;
		}

		timer -= 20.0f;

		movementRight += frameMovementRight;
		movementUp += frameMovementUp;

		if (isGround == false)
		{
			newState = FALLING;
		}

		SetNewAnimation(newState);
		Move();
	}
}

void Player::FixedUpdate()
{

}

void Player::Move()
{
	 m_playerModel->SetTranslation(movementRight, movementUp, 0.0f);
}

void Player::DealDamage(int dmg, D3DXVECTOR3 dmgOrigin)
{
	/*if (health <= 0)
		return;*/
	
	if (invincible)
		return;

	invincible = true;
	invincibilityTimeCurrent = invincibilityTime;
	health -= dmg;
	if (health <= 0)
	{
		health = 0;
		PlayerDeath();
	}

	if (GetModel()->GetTranslation().x < dmgOrigin.x)
		scheduledMovementX = -25.0f;
	else
		scheduledMovementX = 25.0f;

	m_shaderClass->SetColorTint(D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));

	//m_playerModel->SetTranslation(m_playerModel->GetTranslation().x - 25.0f, m_playerModel->GetTranslation().y, m_playerModel->GetTranslation().z);
}

Bounds Player::GetBounds()
{
	return m_playerModel->GetBounds();
}

float Player::GetHealthProgress()
{
	return (float)health / (float)maxHealth;
}

ModelClass * Player::GetModel()
{
	return m_playerModel;
}

void Player::ChangePosition(float x, float y)
{
	movementRight += x;
	movementUp += y;
}

void Player::ResetPlayer()
{
	movementRight = 0;
	movementUp = 0;
	health = maxHealth;
}

void Player::PlayerDeath()
{
	movementUp += 100.0f;
}

void Player::SetNewAnimation(int newState)
{
	if (newState == m_playerAnimation->GetCurrentState())
		return;

	m_playerAnimation->SetState(newState);
	m_graphics->SetPlayerAnimation(m_playerAnimation->GetCurrentState(), m_shaderClass);
}

void Player::OnDestroy()
{

}