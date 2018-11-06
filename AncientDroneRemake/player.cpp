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
	m_playerModel = new ModelClass();
	if (!m_playerModel)
		return false;

	if (m_d3d == nullptr)
		return false;

	if (!m_playerModel->Initialize(m_d3d->GetDevice(), 12, 12))
		return false;

	if (m_graphics == nullptr)
		return false;

	m_playerAnimation = new PlayerAnimationStates();
	if (m_playerAnimation == nullptr)
		return false;

	m_playerModel->SetBounds(-4, 4, m_playerModel->GetBounds().min.y, m_playerModel->GetBounds().max.y - 3);

	m_playerAnimation->PrepareAnimationPose(IDLE, IDLE);
	m_playerAnimation->PrepareAnimationPose(MOVING, MOVING);
	m_playerAnimation->PrepareAnimationPose(COMMAND, COMMAND);
	m_playerAnimation->PrepareAnimationPose(FALLING, FALLING);
	m_playerAnimation->SetState(IDLE);

	m_graphics->SetPlayerModel(m_playerModel);
	movementUp += 10.0f;
	movementRight -= 60.0f;
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
	StatePlayer newState = IDLE;

	if (m_input->IsKeyDown(btn_moveRight))
	{
		frameMovementRight += 1.0f;
	}
	if (m_input->IsKeyDown(btn_moveLeft))
	{
		frameMovementRight -= 1.0f;
	}
	if (frameMovementRight > 0.0f)
	{
		m_playerModel->movingRight = true;
		newState = MOVING;
		idleTime = 0;
	}
	else if (frameMovementRight < 0.0f)
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

	if (m_input->IsKeyDown(btn_jump) && isGround)
	{
		isGround = false;
		currentJumpTicks = jumpTicks;
	}
	
	__int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	timer += (now - lastTime);
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

		for (int i = 0; i < m_graphics->GetGroundModelCount(); i++)
		{
			//float groundThickness = (m_graphics->GetGroundModel(i)->GetBounds().max.y - m_graphics->GetGroundModel(i)->GetBounds().min.y) / 4;

			if (frameMovementUp > 0.0f && //Check if player is moving up
				//Check if player is inside environment #1 - in the middle
				( (m_playerModel->GetBounds().max.y > m_graphics->GetGroundModel(i)->GetBounds().max.y &&
					m_playerModel->GetBounds().min.y < m_graphics->GetGroundModel(i)->GetBounds().min.y) ||
				//Check if player is inside environment #2 - at the top
				(m_playerModel->GetBounds().max.y > m_graphics->GetGroundModel(i)->GetBounds().min.y &&
					m_playerModel->GetBounds().max.y < m_graphics->GetGroundModel(i)->GetBounds().max.y &&
					m_playerModel->GetBounds().min.y < m_graphics->GetGroundModel(i)->GetBounds().min.y))
				//Check if player is in horizontal bounds of environment
				&& m_playerModel->GetBounds().min.x < m_graphics->GetGroundModel(i)->GetBounds().max.x
					&& m_playerModel->GetBounds().max.x > m_graphics->GetGroundModel(i)->GetBounds().min.x)
			{
				isFalling = true;
				currentJumpTicks = 0;
				frameMovementUp = -(m_playerModel->GetBounds().max.y - m_graphics->GetGroundModel(i)->GetBounds().min.y) - 0.1f;
			}
			else if (m_playerModel->GetBounds().min.y <= m_graphics->GetGroundModel(i)->GetBounds().max.y)
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
					}
					else if (m_playerModel->GetBounds().max.x < m_graphics->GetGroundModel(i)->GetBounds().max.x)
					{
						if (m_graphics->GetGroundModel(i)->GetBounds().min.x - m_playerModel->GetBounds().max.x <= 0.0f && frameMovementRight > 0.0f)
							frameMovementRight = m_graphics->GetGroundModel(i)->GetBounds().min.x - m_playerModel->GetBounds().max.x;
					}
					else if (m_playerModel->GetBounds().min.x > m_graphics->GetGroundModel(i)->GetBounds().min.x)
					{
						float temp_ = -(m_playerModel->GetBounds().min.x - m_graphics->GetGroundModel(i)->GetBounds().max.x);

						if (temp_ >= 0.0f && frameMovementRight < 0.0f)
							frameMovementRight = temp_;
					}
				}
			}
		}

		if (isFalling)
			frameMovementUp -= gravity;

		timer -= 20.0f;		
	}

	movementRight += frameMovementRight;
	movementUp += frameMovementUp;

	if (isGround == false)
	{		
		newState = FALLING;
	}

	SetNewAnimation(newState);
	Move();
}

void Player::FixedUpdate()
{

}

void Player::Move()
{
	m_playerModel->SetTranslation(movementRight, movementUp, 0.0f);
}

void Player::DealDamage(int dmg)
{
	/*if (health <= 0)
		return;*/

	health -= dmg;
	if (health <= 0)
	{
		health = 0;
		PlayerDeath();
	}
}

Bounds Player::GetBounds()
{
	return m_playerModel->GetBounds();
}

void Player::PlayerDeath()
{
	movementUp += 100.0f;
}

void Player::SetNewAnimation(StatePlayer newState)
{
	if (newState == m_playerAnimation->GetCurrentState())
		return;

	m_playerAnimation->SetState(newState);
	m_graphics->SetPlayerAnimation(m_playerAnimation->GetCurrentState());
}

void Player::OnDestroy()
{

}