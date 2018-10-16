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
	movementUp += 100.0f;
	movementRight -= 80.0f;
}

bool Player::Initialize(InputClass * inputClass)
{
	m_input = inputClass;
	return Initialize();
}

void Player::Update()
{
	if (m_input->IsKeyDown(btn_moveRight))
	{
		movementRight += 2.0f;
	}
	if (m_input->IsKeyDown(btn_moveLeft))
	{
		movementRight -= 2.0f;
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
		if (currentJumpTicks > 0)
		{
			isGround = false;
			movementUp += jumpTickHeight;
			currentJumpTicks--;
		}
		//else if (movementUp - gravity > -24.0f)
		else if (m_playerModel->GetBounds().min.y - (m_graphics->GetGroundModel()->GetBounds().max.y - m_graphics->GetGroundModel()->GetBounds().min.y) <= m_graphics->GetGroundModel()->GetBounds().max.y
			&& m_playerModel->GetBounds().min.x < m_graphics->GetGroundModel()->GetBounds().max.x)
		{
			float playerMin_ = m_playerModel->GetBounds().min.y - (m_graphics->GetGroundModel()->GetBounds().max.y - m_graphics->GetGroundModel()->GetBounds().min.y);
			float groundMax_ = m_graphics->GetGroundModel()->GetBounds().max.y;

			float playerMinX = m_playerModel->GetBounds().min.x;
			float groundMaxX = m_graphics->GetGroundModel()->GetBounds().max.x;			
			
			movementUp += (groundMax_ - playerMin_);

			isGround = true;			
		}
		else
		{
			isGround = false;
			movementUp -= gravity;
		}

		timer -= 20.0f;		
	}

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