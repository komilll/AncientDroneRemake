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

	if (enemyWanderer)
	{
		enemyWanderer->Update();
		enemyWanderer->TouchedPlayer(player, player->GetBounds().min.x, player->GetBounds().max.x, player->GetBounds().min.y, player->GetBounds().max.y);
	}

	if (enemyFlying)
	{
		enemyFlying->Update();
	}

	if (enemyArcher)
	{
		enemyArcher->Update();
		enemyArcher->TouchedPlayer(player, player->GetBounds().min.x, player->GetBounds().max.x, player->GetBounds().min.y, player->GetBounds().max.y);
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
	//
	//enemyWanderer = new EnemyWanderer();
	//if (enemyWanderer == nullptr)
	//	return false;

	//if (!enemyWanderer->Init(graphicsClass, 16.0f, 16.0f, 0.0f, 0.0f, "mob_spikyback.dds"))
	//	return false;

	//enemyFlying = new EnemyFlying();
	//if (enemyFlying == nullptr)
	//	return false;

	//if (!enemyFlying->Init(graphicsClass, 16, 16))
	//	return false;

	//enemyFlying->SetPlayer(player);
	//enemyFlying->SetWaypoints(D3DXVECTOR2(-120.0f, 0.0f), D3DXVECTOR2(120.0f, 0.0f));

	//enemyArcher = new EnemyArcher();
	//if (enemyArcher == nullptr)
	//	return false;

	//if (!enemyArcher->Init(graphicsClass, 12, 12))
	//	return false;

	//enemyArcher->SetPlayer(player);

	droneController = new DroneController();
	if (droneController == nullptr)
		return false;

	if (!droneController->Init(graphicsClass, 12.0f, 12.0f, 0.0f, 15.0f, "ancient_ball.dds"))
		return false;

	return true;
}

void GameManager::LMBPressed()
{
	if (droneController)
		droneController->Attack();
}