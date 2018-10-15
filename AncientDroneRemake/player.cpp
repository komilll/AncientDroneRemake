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
		movementRight += 5.0f;
	}
	if (m_input->IsKeyDown(btn_moveLeft))
	{
		movementRight -= 5.0f;
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
		else if (movementUp - gravity > -24.0f)
		{
			isGround = false;
			movementUp -= gravity;
		}
		else
			isGround = true;

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