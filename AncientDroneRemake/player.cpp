#include "player.h"

Player::Player()
{
	m_d3d = 0;
	m_input = 0;
	m_playerModel = 0;
	timer = 0;
	lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
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

	if (!m_playerModel->Initialize(m_d3d->GetDevice(), 8, 16))
		return false;

	if (m_graphics == nullptr)
		return false;

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

	if (m_input->IsKeyDown(btn_moveRight))
	{
		frameMovementRight += 1.0f;
	}
	if (m_input->IsKeyDown(btn_moveLeft))
	{
		frameMovementRight -= 1.0f;
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

		for (int i = 0; i < 4; i++)
		{
			float groundThickness = (m_graphics->GetGroundModel(i)->GetBounds().max.y - m_graphics->GetGroundModel(i)->GetBounds().min.y) / 4;

			if (currentJumpTicks > 0)
			{
				isFalling = false;

				frameMovementUp += jumpTickHeight;
				currentJumpTicks--;
			}
			if (m_playerModel->GetBounds().min.y - groundThickness <= m_graphics->GetGroundModel(i)->GetBounds().max.y)
			{
				//float playerMin_ = m_playerModel->GetBounds().min.y - groundThickness;
				//float groundMax_ = m_graphics->GetGroundModel()->GetBounds().max.y;

				//float playerMinX = m_playerModel->GetBounds().min.x;
				//float groundMaxX = m_graphics->GetGroundModel()->GetBounds().max.x;

				//frameMovementUp += (groundMax_ - playerMin_);

				if (m_playerModel->GetBounds().max.y + groundThickness > m_graphics->GetGroundModel(i)->GetBounds().min.y)
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

	Move();
}

void Player::FixedUpdate()
{

}

void Player::Move()
{
	m_playerModel->SetTranslation(movementRight, movementUp, 0.0f);
}

void Player::OnDestroy()
{

}