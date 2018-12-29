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

	for (int i = 0; i < m_enemyWanderer.size(); i++)
	{
		m_enemyWanderer.at(i)->Update();
		m_enemyWanderer.at(i)->TouchedPlayer(player, player->GetBounds().min.x, player->GetBounds().max.x, player->GetBounds().min.y, player->GetBounds().max.y);
	}

	for (int i = 0; i < m_enemyFlying.size(); i++)
	{
		m_enemyFlying.at(i)->Update();
	}

	for (int i = 0; i < m_enemyArcher.size(); i++)
	{
		m_enemyArcher.at(i)->Update();
		m_enemyArcher.at(i)->TouchedPlayer(player, player->GetBounds().min.x, player->GetBounds().max.x, player->GetBounds().min.y, player->GetBounds().max.y);
	}

	if (droneController)
	{
		droneController->Update();
		for (int i = 0; i < m_enemyWanderer.size(); i++)
			droneController->CheckSpearsDamage(m_enemyWanderer.at(i));
		for (int i = 0; i < m_enemyFlying.size(); i++)
			droneController->CheckSpearsDamage(m_enemyFlying.at(i));
		for (int i = 0; i < m_enemyArcher.size(); i++)
			droneController->CheckSpearsDamage(m_enemyArcher.at(i));
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

	for (int i = 0; i < m_darkSphere.size(); i++) 
	{
		m_darkSphere.at(i)->Update();
		m_darkSphere.at(i)->TouchedPlayer(player, player->GetBounds().min.x, player->GetBounds().max.x, player->GetBounds().min.y, player->GetBounds().max.y);
	}

	if (m_levelFinish)
	{
		m_levelFinish->Update();
		m_levelFinish->TouchedPlayer(player, player->GetBounds().min.x, player->GetBounds().max.x, player->GetBounds().min.y, player->GetBounds().max.y);
	}
}

bool GameManager::Initialize(InputClass *inputClass, MouseClass* mouseClass, D3DClass *d3d, GraphicsClass *graphicsClass)
{	
	m_inputClass = inputClass;
	m_graphics = graphicsClass;
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

	player->GetModel()->SetVisibility(false);

	return true;
}

void GameManager::LMBPressed()
{
	if (droneController && m_gameStarted)
		droneController->Attack();
}

void GameManager::RMBPressed()
{
	if (droneController && m_gameStarted)
		droneController->AttackExplosion();
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
		droneController->FreeDroneFromPlayer();
		droneController->SetDestination(destX, destY);
	}
}

void GameManager::CallDroneToPlayer()
{
	if (droneController)
	{
		droneController->CallDroneToPlayer();
	}
}

Player * GameManager::GetPlayer()
{
	return player;
}

bool GameManager::SpawnObjects()
{
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

	if (!droneController->Init(m_graphics, 12.0f, 12.0f, 0.0f, 15.0f, "ancient_ball.dds"))
		return false;
	droneController->GetModel()->SetVisibility(false);

	////////////////////////////////
	////////////// UI //////////////
	////////////////////////////////

#pragma region Gameplay UI
	healthBarBackground = new UIController();
	if (healthBarBackground == nullptr)
		return false;

	healthBarBackground->Init(m_graphics, 6 * 8, 6, 512, 64, -85.0f, 90.0f, "UIBackground.dds");

	healthBar = new UIController();
	if (healthBar == nullptr)
		return false;

	healthBar->Init(m_graphics, 6 * 8, 6, 512, 64, -85.0f, 90.25f, "UIHealth.dds");

	progressBarBackground = new UIController();
	if (progressBarBackground == nullptr)
		return false;

	progressBarBackground->Init(m_graphics, 6 * 8, 6, 512, 64, -85.0f, 75.0f, "UIBackground.dds");

	progressBar = new UIController();
	if (progressBar == nullptr)
		return false;

	progressBar->Init(m_graphics, 6 * 8, 6, 512, 64, -85.0f, 75.0f, "UIProgress.dds");

	healthBarBackground->GetModel()->SetVisibility(false);
	healthBar->GetModel()->SetVisibility(false);
	progressBarBackground->GetModel()->SetVisibility(false);
	progressBar->GetModel()->SetVisibility(false);
#pragma endregion

	#pragma region Menu UI
	menuTitle = new UIController();
	if (menuTitle == nullptr)
		return false;

	menuTitle->Init(m_graphics, 6 * 4, 6, 256, 64, 0.0f, 60.0f, "UIMenu_Title.dds");
	menuTitle->GetModel()->SetScale(3.0f, 3.0f, 3.0f);

	if (menuStartGame == nullptr)
	{
		menuStartGame = new UIController();
		if (menuStartGame == nullptr)
			return false;
	}

	menuStartGame->Init(m_graphics, 6 * 4, 6, 256, 64, 0.0f, -15.0f, "UIMenu_StartGame.dds");
	menuStartGame->GetModel()->SetScale(2.0f, 2.0f, 2.0f);
	menuStartGame->InitializeButton(m_mouseClass);
	menuStartGame->EventOnPressButton = [=]() -> void { StartGame(); };

	menuQuit = new UIController();
	if (menuQuit == nullptr)
		return false;

	menuQuit->Init(m_graphics, 6 * 4, 6, 256, 64, 0.0f, -45.0f, "UIMenu_Quit.dds");
	menuQuit->GetModel()->SetScale(2.0f, 2.0f, 2.0f);
	menuQuit->InitializeButton(m_mouseClass);
	menuQuit->EventOnPressButton = []()-> void { PostQuitMessage(0); };

	endGameTitle = new UIController();
	if (endGameTitle == nullptr)
		return false;

	endGameTitle->Init(m_graphics, 142, 106.5f, 800, 600, 0.0f, 0.0f, "end_screen.dds");
	endGameTitle->GetModel()->SetVisibility(false);
	#pragma endregion
}

UIController * GameManager::GetMenuStartGame()
{
	return menuStartGame;
}

void GameManager::SetMenuStartGame()
{
	menuStartGame = new UIController();
}

void GameManager::StartGame()
{
	StartGameBuildLevel();
	droneController->GetModel()->SetVisibility(true);
	player->GetModel()->SetVisibility(true);
	droneController->GetModel()->SetTranslation(player->GetModel()->GetTranslation().x, player->GetModel()->GetTranslation().y, player->GetModel()->GetTranslation().z);
	menuQuit->GetModel()->SetVisibility(false);
	menuQuit->EnableButton(false);

	menuStartGame->GetModel()->SetVisibility(false);
	menuStartGame->EnableButton(false);

	menuTitle->GetModel()->SetVisibility(false);

	healthBarBackground->GetModel()->SetVisibility(true);
	healthBar->GetModel()->SetVisibility(true);
	progressBarBackground->GetModel()->SetVisibility(true);
	progressBar->GetModel()->SetVisibility(true);

	m_mouseClass->SetLMBPressed(false);
	m_gameStarted = true;
	
	m_graphics->GetMainMenuBackground()->SetVisibility(false);
}

void GameManager::RestartLevel()
{
	droneController->SetDroneFullEnergy();
	droneController->CallDroneToPlayer();
	droneController->GetModel()->SetTranslation(player->GetModel()->GetTranslation().x, player->GetModel()->GetTranslation().y, player->GetModel()->GetTranslation().z);
}

void GameManager::AddNewEnemy(EnemyType type, void* enemy)
{
	PushNewEnemy(type, enemy);
}

void * GameManager::GetEnemy(EnemyType type, int index)
{
	return GetEnemyLocal(type, index);
}

void GameManager::AddDarkSphere(DarkSphere* darkSphere)
{
	m_darkSphere.push_back(darkSphere);
}

void GameManager::SetLevelFinish(LevelFinish * levelFinish)
{
	m_levelFinish = levelFinish;
}

LevelFinish * GameManager::GetLevelFinish()
{
	return m_levelFinish;
}

bool GameManager::CheckNextLevel()
{
	if (!m_levelFinish)
		return false;
	
	if (m_levelFinish->ReadyForNextLevel())
	{
		if (m_levelFinish->m_currentLevel == 2)
		{
			DestroyLevel();
			FinishedGame();
			return false;
		}
		
		DestroyLevel();
		return true;
	}

	return false;
}

void GameManager::FinishedGame()
{
	endGameTitle->GetModel()->SetVisibility(true);
}

void GameManager::UseGodMode()
{
	player->UseGodMode();
}

void GameManager::PushNewEnemy(EnemyType enemyType, void* enemy)
{
	if (enemyType == G_WANDERER)
		m_enemyWanderer.push_back(static_cast<EnemyWanderer*>(enemy));	
	else if (enemyType == G_ARCHER)
		m_enemyArcher.push_back(static_cast<EnemyArcher*>(enemy));
	else if (enemyType == G_FLYING)
		m_enemyFlying.push_back(static_cast<EnemyFlying*>(enemy));
}

void * GameManager::GetEnemyLocal(EnemyType type, int index)
{
	if (type == G_WANDERER) //WANDERER
		return m_enemyWanderer.at(index);
	else if (type == G_ARCHER) //ARCHER
		return m_enemyArcher.at(index);
	else if (type == G_FLYING) //CROW
		return m_enemyFlying.at(index);
	else
		MessageBox(*m_graphics->GetHWND(), "No such class of enemy exists", "Type error", MB_OK);
}

void GameManager::DestroyLevel()
{
	for (int i = 0; i < m_enemyWanderer.size(); i++)
	{
		m_enemyWanderer.at(i)->Shutdown();
		delete m_enemyWanderer.at(i);
	}
	m_enemyWanderer.clear();

	for (int i = 0; i < m_enemyFlying.size(); i++)
	{
		m_enemyFlying.at(i)->Shutdown();
		delete m_enemyFlying.at(i);
	}
	m_enemyFlying.clear();

	for (int i = 0; i < m_enemyArcher.size(); i++)
	{
		m_enemyArcher.at(i)->Shutdown();
		delete m_enemyArcher.at(i);
	}
	m_enemyArcher.clear();

	for (int i = 0; i < m_darkSphere.size(); i++)
	{
		m_darkSphere.at(i)->Shutdown();
		delete m_darkSphere.at(i);
	}
	m_darkSphere.clear();

	m_levelFinish->Shutdown();
	delete m_levelFinish;
	m_levelFinish = nullptr;
}