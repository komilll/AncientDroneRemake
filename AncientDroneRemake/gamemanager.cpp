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

	if (droneController)
	{
		droneController->Update();
		droneController->CheckSpearsDamage(enemyWanderer);
		droneController->CheckSpearsDamage(enemyFlying);
		droneController->CheckSpearsDamage(enemyArcher);
	}
	
	if (healthBar && player)
	{
		healthBar->SetProgress(player->GetHealthProgress());
	}

	if (progressBar && droneController)
	{
		progressBar->SetProgress(droneController->GetDroneEnergyProgress());
	}

	if (menuStartGame)
	{
		menuStartGame->Update();
	}

	if (menuQuit)
	{
		menuQuit->Update();
	}
}

bool GameManager::Initialize(InputClass *inputClass, MouseClass* mouseClass, D3DClass *d3d, GraphicsClass *graphicsClass)
{	
	m_inputClass = inputClass;
	if (m_inputClass == nullptr)
		return false;

	m_mouseClass = mouseClass;
	if (m_mouseClass == nullptr)
		return false;

	player = new Player(m_inputClass, d3d, graphicsClass);
	if (player == nullptr)
		return false;

	if (!player->Initialize())
		return false;
	
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

	////////////////////////////////
	////////////// UI //////////////
	////////////////////////////////

	#pragma region Gameplay UI
	healthBarBackground = new UIController();
	if (healthBarBackground == nullptr)
		return false;

	healthBarBackground->Init(graphicsClass, 6*8, 6, 512, 64, -85.0f, 90.0f, "UIBackground.dds");

	healthBar = new UIController();
	if (healthBar == nullptr)
		return false;

	healthBar->Init(graphicsClass, 6 * 8, 6, 512, 64, -85.0f, 90.25f, "UIHealth.dds");	

	progressBarBackground = new UIController();
	if (progressBarBackground == nullptr)
		return false;

	progressBarBackground->Init(graphicsClass, 6 * 8, 6, 512, 64, -85.0f, 75.0f, "UIBackground.dds");

	progressBar = new UIController();
	if (progressBar == nullptr)
		return false;

	progressBar->Init(graphicsClass, 6 * 8, 6, 512, 64, -85.0f, 75.0f, "UIProgress.dds");
#pragma endregion

/*
	#pragma region Menu UI
	menuTitle = new UIController();
	if (menuTitle == nullptr)
		return false;

	menuTitle->Init(graphicsClass, 6 * 4, 6, 256, 64, 0.0f, 60.0f, "UIMenu_Title.dds");
	menuTitle->GetModel()->SetScale(3.0f, 3.0f, 3.0f);

	menuStartGame = new UIController();
	if (menuStartGame == nullptr)
		return false;

	menuStartGame->Init(graphicsClass, 6 * 4, 6, 256, 64, 0.0f, -15.0f, "UIMenu_StartGame.dds");
	menuStartGame->GetModel()->SetScale(2.0f, 2.0f, 2.0f);
	menuStartGame->InitializeButton(m_mouseClass);
	menuStartGame->EventOnPressButton = [=]() -> void { StartGame(); };

	menuQuit = new UIController();
	if (menuQuit == nullptr)
		return false;

	menuQuit->Init(graphicsClass, 6 * 4, 6, 256, 64, 0.0f, -45.0f, "UIMenu_Quit.dds");
	menuQuit->GetModel()->SetScale(2.0f, 2.0f, 2.0f);
	menuQuit->InitializeButton(m_mouseClass);
	menuQuit->EventOnPressButton = []()-> void { PostQuitMessage(0); };

#pragma endregion
*/

	return true;
}

void GameManager::LMBPressed()
{
	if (droneController)
		droneController->Attack();
}

void GameManager::SetDroneRotation(float mousePosX, float mousePosY)
{
	if (droneController)
	{
		float droneMidX = droneController->GetModel()->GetBounds().min.x + 
			(droneController->GetModel()->GetBounds().max.x - droneController->GetModel()->GetBounds().min.x) / 2.0f;
		float droneMidY = droneController->GetModel()->GetBounds().min.y +
			(droneController->GetModel()->GetBounds().max.y - droneController->GetModel()->GetBounds().min.y) / 2.0f;

		float angle = atan2(mousePosX - droneMidX, -(mousePosY - droneMidY)) - 3.14f / 2.0f;
		//angle = angle * 180 / 3.14f;
		//angle -= 90.0f;
		droneController->GetModel()->SetRotation(angle);
	}
}

void GameManager::SetDroneDestination(float destX, float destY)
{
	if (droneController)
	{
		droneController->SetDestination(destX, destY);
	}
}

void GameManager::StartGame()
{
}

Player * GameManager::GetPlayer()
{
	return player;
}
