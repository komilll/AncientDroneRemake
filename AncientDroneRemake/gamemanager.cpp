#include "gamemanager.h"

GameManager::GameManager()
{
	//m_systemClass = 0;
	//player = 0;
}

GameManager::GameManager(const GameManager &)
{

}

GameManager::~GameManager()
{

}


void GameManager::Update()
{
	if (player)
	{
		player->Update();
	}

	if (enemy)
	{
		enemy->Update();
		enemy->TouchedPlayer(player, player->GetBounds().min.x, player->GetBounds().max.x, player->GetBounds().min.y, player->GetBounds().max.y);
	}
}

bool GameManager::Initialize(InputClass *inputClass, D3DClass *d3d, GraphicsClass *graphicsClass)
{
	m_inputClass = inputClass;
	if (m_inputClass == nullptr)
		return false;

	player = new Player(m_inputClass, d3d, graphicsClass);
	if (player == nullptr)
		return false;

	if (!player->Initialize())
		return false;
	
	enemy = new EnemyWanderer();
	if (enemy == nullptr)
		return false;

	if (!enemy->Init(graphicsClass))
		return false;

	return true;
}
