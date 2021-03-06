#include "tiledinterpreter.h"

TiledInterpreter::TiledInterpreter()
{

}

void TiledInterpreter::Initialize(GraphicsClass * graphicsClass, Player* player, GameManager* gameManager)
{
	m_graphics = graphicsClass;
	m_player = player;
	m_gameManager = gameManager;

	for (int i = 0; i < 8; i++)
	{
		TextureShaderGeneralClass* newTexture = new TextureShaderGeneralClass();
		newTexture->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND(), (CHAR*)GetTileName(i + 1).c_str());
		m_textureGeneral.push_back(newTexture);
		m_graphics->AddTextureShaderGeneral(newTexture);
	}

	ReadMapFile();
}

void TiledInterpreter::Import(bool restart)
{
	m_wandererIndex = 0;
	m_archerIndex = 0;
	m_crowIndex = 0;

	if (restart == false)
	{
		for (int i = 0; i < 8; i++)
		{
			m_textureGeneral.at(i)->ClearModels();
			m_graphics->ClearGrounds();
		}

		for (int i = 0; i < MAP_WIDTH; i++)
		{
			for (int k = 0; k < MAP_HEIGHT; k++)
			{
				if (tab[i][k] >= TILE_MIN && tab[i][k] <= TILE_MAX)
					SpawnTile(i - firstTile, MAP_HEIGHT - k, tab[i][k]);
				else if (tab[i][k] == DARK_SPHERE)
					SpawnDarkSphere(TILE_SIZE * (i - firstTile) * 2, (MAP_HEIGHT - k) * TILE_SIZE * 2 - TILE_SIZE * 100 * 2);
				else if (tab[i][k] == SPAWN_POINT)
					SpawnPlayer(TILE_SIZE * (i - firstTile) * 2, (MAP_HEIGHT - k) * TILE_SIZE * 2);
				else if ((tab[i][k] >= WANDERER && tab[i][k] <= CROW) || tab[i][k] == WANDERER_VERTICAL)
					SpawnEnemy(i - firstTile, MAP_HEIGHT - k, tab[i][k]);
				else if (tab[i][k] == END_POINT)
					SpawnLevelFinish(TILE_SIZE * (i - firstTile) * 2, (MAP_HEIGHT - k) * TILE_SIZE * 2 - TILE_SIZE * 100 * 2);
			}
		}
		if (m_backgroundSpawned == false)
		{
			SpawnBackgroundsContinue();
			m_startBackgroundSpawnTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		}
	}
	else
	{
		for (int i = 0; i < MAP_WIDTH; i++)
		{
			for (int k = 0; k < MAP_HEIGHT; k++)
			{
				if (tab[i][k] == SPAWN_POINT)
					SpawnPlayer(TILE_SIZE * (i - firstTile) * 2, (MAP_HEIGHT - k) * TILE_SIZE * 2);
				else if (tab[i][k] >= WANDERER && tab[i][k] <= CROW)
					SpawnEnemy(i - firstTile, MAP_HEIGHT - k, tab[i][k], true);
			}
		}
	}
}

void TiledInterpreter::RestartLevel()
{
	Import(true);
}

void TiledInterpreter::LoadNextLevel()
{
	m_startBackgroundSpawnTime = -1;
	//DestroyBackgrounds();
	if (m_levelIndex == 2)
		return;
	m_levelIndex++;
	ReadMapFile();
	Import();
}

void TiledInterpreter::Update()
{
	if (m_startBackgroundSpawnTime != -1)
		SpawnBackgrounds();
}

void TiledInterpreter::SpawnTile(int indexX, int indexY, int indexTile)
{	
	indexY -= 100;
	indexTile = indexTile - TILE_MIN;
	m_textureGeneral.at(indexTile)->AddModel(m_graphics->AddGroundModel(TILE_SIZE, TILE_SIZE, TILE_SIZE * indexX * 2, TILE_SIZE * indexY * 2));
}

void TiledInterpreter::ReadMapFile()
{
	ifstream iFile;
	//ofstream oFile;

	iFile.open(GetLevelToLoad(m_levelIndex));
	//oFile.open("map_test.txt");
	//oFile.write("", 0);

	std::string line;
	std::string msg;
	char* test;
	int a;

	int row = 0;
	int column = 0;
	while (std::getline(iFile, line))
	{
		for (int i = 0; i < line.size(); i++)
		{
			if (line.at(i) == ',')
			{
				tab[column][row] = std::stoi(msg);
				msg = "";
				column++;
			}
			else
				msg += line.at(i);
		}
		row++;
		column = 0;
	}

	iFile.close();
	FindFirstTileX();
	//Read waypoints map
	iFile.open(GetWaypointToLoad(m_levelIndex));

	line = "";
	msg = "";
	test = 0;
	a = 0;
	row = 0;
	column = 0;
	while (std::getline(iFile, line))
	{
		for (int i = 0; i < line.size(); i++)
		{
			if (line.at(i) == ',')
			{
				if (std::stoi(msg) == WAYPOINT)				
					m_waypoints.push_back(D3DXVECTOR2((column - firstTile) * TILE_SIZE * 2, (MAP_HEIGHT - row - 100) * TILE_SIZE * 2));
				
				msg = "";
				column++;
			}
			else
				msg += line.at(i);
		}
		row++;
		column = 0;
	}

	iFile.close();
}

void TiledInterpreter::SpawnPlayer(float posX, float posY)
{	
	m_player->ResetPlayer();
	m_player->ChangePosition(posX, posY - TILE_SIZE * 100 * 2);	
	m_player->Move();
}

void TiledInterpreter::SpawnEnemy(int indexX, int indexY, int indexEnemy, bool restart)
{
	indexY -= 100;

	EnemyWanderer* wanderer = nullptr;
	EnemyArcher* archer = nullptr;
	EnemyFlying* flying = nullptr;

	switch (indexEnemy)
	{
		case WANDERER_VERTICAL:
			indexY--;
			if (restart)
			{
				((EnemyWanderer*)(m_gameManager->GetEnemy(G_WANDERER, m_wandererIndex)))->GetModel()->SetTranslation(TILE_SIZE * indexX * 2, TILE_SIZE * indexY * 2, 0.0f);
				((EnemyWanderer*)(m_gameManager->GetEnemy(G_WANDERER, m_wandererIndex)))->RestartEnemy();
				m_wandererIndex++;
			}
			else
			{
				wanderer = new EnemyWanderer();
				wanderer->Init(m_graphics, 16.0f, 16.0f, TILE_SIZE * indexX * 2, TILE_SIZE * indexY * 2, "mob_spikyback.dds");
				m_gameManager->AddNewEnemy(G_WANDERER, wanderer);
				wanderer->ForceVerticalMovement();
			}
			break;
		case WANDERER:
			indexY--;
			if (restart)
			{
				((EnemyWanderer*)(m_gameManager->GetEnemy(G_WANDERER, m_wandererIndex)))->GetModel()->SetTranslation(TILE_SIZE * indexX * 2, TILE_SIZE * indexY * 2, 0.0f);
				((EnemyWanderer*)(m_gameManager->GetEnemy(G_WANDERER, m_wandererIndex)))->RestartEnemy();
				m_wandererIndex++;
			}
			else
			{
				wanderer = new EnemyWanderer();
				wanderer->Init(m_graphics, 16.0f, 16.0f, TILE_SIZE * indexX * 2, TILE_SIZE * indexY * 2, "mob_spikyback.dds");
				m_gameManager->AddNewEnemy(G_WANDERER, wanderer);
			}
			break;
		case ARCHER:
			indexY--;
			if (restart)
			{
				((EnemyArcher*)(m_gameManager->GetEnemy(G_ARCHER, m_archerIndex)))->GetModel()->SetTranslation(TILE_SIZE * indexX * 2, TILE_SIZE * indexY * 2, 0.0f);
				((EnemyArcher*)(m_gameManager->GetEnemy(G_ARCHER, m_archerIndex)))->RestartEnemy();
				m_archerIndex++;
			}
			else
			{
				archer = new EnemyArcher();
				archer->Init(m_graphics, 12.0f, 12.0f, TILE_SIZE * indexX * 2, TILE_SIZE * indexY * 2);
				archer->SetPlayer(m_player);
				m_gameManager->AddNewEnemy(G_ARCHER, archer);
			}
			break;
		case CROW:
			if (restart)
			{
				((EnemyFlying*)(m_gameManager->GetEnemy(G_FLYING, m_crowIndex)))->GetModel()->SetTranslation(TILE_SIZE * indexX * 2, TILE_SIZE * indexY * 2, 0.0f);
				((EnemyFlying*)(m_gameManager->GetEnemy(G_FLYING, m_crowIndex)))->RestartEnemy();
				m_crowIndex++;
			}
			else
			{
				flying = new EnemyFlying();
				flying->Init(m_graphics, 16, 16, TILE_SIZE * indexX * 2, TILE_SIZE * indexY * 2);
				flying->SetPlayer(m_player);
				//enemyFlying->SetWaypoints(D3DXVECTOR2(-120.0f, 0.0f), D3DXVECTOR2(120.0f, 0.0f));
				int indexRight = -1;
				int indexLeft = -1;
				float posX = TILE_SIZE * indexX * 2;
				for (int i = 0; i < m_waypoints.size(); i++)
				{
					if (m_waypoints.at(i).y == TILE_SIZE * indexY * 2)
					{
						if (indexRight == -1 && m_waypoints.at(i).x > posX)
							indexRight = i;
						else if (indexLeft == -1 && m_waypoints.at(i).x < posX)
							indexLeft = i;
						else if (indexRight != -1 && abs(m_waypoints.at(i).x - posX) < abs(m_waypoints.at(indexRight).x - posX) && m_waypoints.at(i).x > posX)
							indexRight = i;
						else if (indexLeft != -1 && abs(m_waypoints.at(i).x - posX) < abs(m_waypoints.at(indexLeft).x - posX) && m_waypoints.at(i).x < posX)
							indexLeft = i;
					}
				}
				if (indexLeft != -1 && indexRight != -1)
					flying->SetWaypoints(D3DXVECTOR2(m_waypoints.at(indexLeft).x, TILE_SIZE * indexY * 2), D3DXVECTOR2(m_waypoints.at(indexRight).x, TILE_SIZE * indexY * 2));

				m_gameManager->AddNewEnemy(G_FLYING, flying);
			}
			break;
	}

}

void TiledInterpreter::SpawnDarkSphere(float posX, float posY, bool restart)
{
	DarkSphere* darkSphere = new DarkSphere();
	darkSphere->Init(m_graphics, posX, posY);
	m_darkSpheres.push_back(darkSphere);
	m_gameManager->AddDarkSphere(darkSphere);
}

void TiledInterpreter::SpawnLevelFinish(float posX, float posY, bool restart)
{
	if (m_gameManager->GetLevelFinish() == nullptr)
	{
		LevelFinish* levelFinish = new LevelFinish();
		levelFinish->Init(m_graphics, posX, posY);
		levelFinish->m_currentLevel = m_levelIndex;
		m_gameManager->SetLevelFinish(levelFinish);
	}
}

void TiledInterpreter::FindFirstTileX()
{
	//Find most-left tile to use as start point
	for (int i = 0; i < MAP_WIDTH; i++)
	{
		for (int k = 0; k < MAP_HEIGHT; k++)
		{
			if (tab[i][k] >= TILE_MIN && tab[i][k] <= TILE_MAX)
			{
				firstTile = i;
				break;
			}
		}
		if (firstTile > 0)
			break;
	}

}

string TiledInterpreter::GetTileName(int index)
{
	switch (index)
	{
		case 1:
			return "Tile_1.dds";
		case 2:
			return "Tile_2.dds";
		case 3:
			return "Tile_3.dds";
		case 4:
			return "Tile_4.dds";
		case 5:
			return "Tile_5.dds";
		case 6:
			return "Tile_6.dds";
		case 7:
			return "Tile_7.dds";
		case 8:
			return "Tile_8.dds";
	}
}

string TiledInterpreter::GetLevelToLoad(int index)
{
	switch (index)
	{
		case 0:
			return level_01;
		case 1:
			return level_02;
		case 2:
			return level_03;
	}

	PostQuitMessage(0);
}

string TiledInterpreter::GetWaypointToLoad(int index)
{
	switch (index)
	{
		case 0:
			return levelWaypont_01;
		case 1:
			return levelWaypont_02;
		case 2:
			return levelWaypont_03;
	}

	PostQuitMessage(0);
}

void TiledInterpreter::SpawnBackgrounds()
{
	if (m_graphics->GetBackgroundShader(6) == nullptr)
		return;

	m_graphics->GetBackgroundShader(0)->GetModels().at(0)->SetTranslation(m_graphics->GetPlayerPosition().x, m_graphics->GetPlayerPosition().y, m_graphics->GetPlayerPosition().z);
	m_graphics->GetBackgroundShader(1)->GetModels().at(0)->SetTranslation(m_graphics->GetPlayerPosition().x + 395, m_graphics->GetPlayerPosition().y, m_graphics->GetPlayerPosition().z);
	m_graphics->GetBackgroundShader(2)->GetModels().at(0)->SetTranslation(m_graphics->GetPlayerPosition().x, m_graphics->GetPlayerPosition().y, m_graphics->GetPlayerPosition().z);
	m_graphics->GetBackgroundShader(3)->GetModels().at(0)->SetTranslation(m_graphics->GetPlayerPosition().x + 395, m_graphics->GetPlayerPosition().y, m_graphics->GetPlayerPosition().z);
	m_graphics->GetBackgroundShader(4)->GetModels().at(0)->SetTranslation(m_graphics->GetPlayerPosition().x, m_graphics->GetPlayerPosition().y - 100, m_graphics->GetPlayerPosition().z);
	m_graphics->GetBackgroundShader(5)->GetModels().at(0)->SetTranslation(m_graphics->GetPlayerPosition().x + 395, m_graphics->GetPlayerPosition().y - 100, m_graphics->GetPlayerPosition().z);
	m_graphics->GetBackgroundShader(6)->GetModels().at(0)->SetTranslation(m_graphics->GetPlayerPosition().x + 795, m_graphics->GetPlayerPosition().y - 100, m_graphics->GetPlayerPosition().z);

	if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - m_startBackgroundSpawnTime > 1000)
		m_startBackgroundSpawnTime = -1;
}

void TiledInterpreter::SpawnBackgroundsContinue()
{
	m_backgroundSpawned = true;

	TextureShaderGeneralClass* backgroundShader = new TextureShaderGeneralClass();
	backgroundShader->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND(), "background_sky.dds");
	m_graphics->AddBackgroundShader(backgroundShader);
	ModelClass *backgroundModel = new ModelClass;
	backgroundModel->Initialize(m_graphics->GetD3D()->GetDevice(), 200, 112.5f);
	backgroundModel->SetTranslation(m_graphics->GetPlayerPosition().x, m_graphics->GetPlayerPosition().y, m_graphics->GetPlayerPosition().z);
	backgroundShader->AddModel(backgroundModel);
	backgroundShader->SetAsTransparent(true);
	backgroundShader->SetConstReverseX(1);

	backgroundShader = new TextureShaderGeneralClass();
	backgroundShader->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND(), "background_sky.dds");
	m_graphics->AddBackgroundShader(backgroundShader);
	backgroundModel = new ModelClass;
	backgroundModel->Initialize(m_graphics->GetD3D()->GetDevice(), 200, 112.5f);
	backgroundModel->SetTranslation(m_graphics->GetPlayerPosition().x + 395, m_graphics->GetPlayerPosition().y, m_graphics->GetPlayerPosition().z);
	backgroundShader->AddModel(backgroundModel);
	backgroundShader->SetAsTransparent(true);
	backgroundShader->SetConstReverseX(-1);
	///////////////////////////////////////////
	backgroundShader = new TextureShaderGeneralClass();
	backgroundShader->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND(), "background_mountains.dds");
	m_graphics->AddBackgroundShader(backgroundShader);
	backgroundModel = new ModelClass;
	backgroundModel->Initialize(m_graphics->GetD3D()->GetDevice(), 200, 112.5f);
	backgroundModel->SetTranslation(m_graphics->GetPlayerPosition().x, m_graphics->GetPlayerPosition().y, m_graphics->GetPlayerPosition().z);
	backgroundShader->AddModel(backgroundModel);
	backgroundShader->SetAsTransparent(true);
	backgroundShader->SetConstReverseX(1);

	backgroundShader = new TextureShaderGeneralClass();
	backgroundShader->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND(), "background_mountains.dds");
	m_graphics->AddBackgroundShader(backgroundShader);
	backgroundModel = new ModelClass;
	backgroundModel->Initialize(m_graphics->GetD3D()->GetDevice(), 200, 112.5f);
	backgroundModel->SetTranslation(m_graphics->GetPlayerPosition().x + 395, m_graphics->GetPlayerPosition().y, m_graphics->GetPlayerPosition().z);
	backgroundShader->AddModel(backgroundModel);
	backgroundShader->SetAsTransparent(true);
	backgroundShader->SetConstReverseX(-1);

	///////////////////////////////////////////
	backgroundShader = new TextureShaderGeneralClass();
	backgroundShader->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND(), "background_sand.dds");
	m_graphics->AddBackgroundShader(backgroundShader);
	backgroundModel = new ModelClass;
	backgroundModel->Initialize(m_graphics->GetD3D()->GetDevice(), 200, 112.5f);
	backgroundModel->SetTranslation(m_graphics->GetPlayerPosition().x, m_graphics->GetPlayerPosition().y - 100, m_graphics->GetPlayerPosition().z);
	backgroundShader->AddModel(backgroundModel);
	backgroundShader->SetAsTransparent(true);
	backgroundShader->SetConstReverseX(1);

	backgroundShader = new TextureShaderGeneralClass();
	backgroundShader->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND(), "background_sand.dds");
	m_graphics->AddBackgroundShader(backgroundShader);
	backgroundModel = new ModelClass;
	backgroundModel->Initialize(m_graphics->GetD3D()->GetDevice(), 200, 112.5f);
	backgroundModel->SetTranslation(m_graphics->GetPlayerPosition().x + 395, m_graphics->GetPlayerPosition().y - 100, m_graphics->GetPlayerPosition().z);
	backgroundShader->AddModel(backgroundModel);
	backgroundShader->SetAsTransparent(true);
	backgroundShader->SetConstReverseX(-1);

	backgroundShader = new TextureShaderGeneralClass();
	backgroundShader->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND(), "background_sand.dds");
	m_graphics->AddBackgroundShader(backgroundShader);
	backgroundModel = new ModelClass;
	backgroundModel->Initialize(m_graphics->GetD3D()->GetDevice(), 200, 112.5f);
	backgroundModel->SetTranslation(m_graphics->GetPlayerPosition().x + 795, m_graphics->GetPlayerPosition().y - 100, m_graphics->GetPlayerPosition().z);
	backgroundShader->AddModel(backgroundModel);
	backgroundShader->SetAsTransparent(true);
	backgroundShader->SetConstReverseX(1);
	//else
	//{
	//	m_graphics->GetBackgroundShader(0)->GetModels().at(0)->SetTranslation(m_graphics->GetPlayerPosition().x, m_graphics->GetPlayerPosition().y, m_graphics->GetPlayerPosition().z);
	//	m_graphics->GetBackgroundShader(1)->GetModels().at(0)->SetTranslation(m_graphics->GetPlayerPosition().x + 395, m_graphics->GetPlayerPosition().y, m_graphics->GetPlayerPosition().z);
	//	m_graphics->GetBackgroundShader(2)->GetModels().at(0)->SetTranslation(m_graphics->GetPlayerPosition().x, m_graphics->GetPlayerPosition().y, m_graphics->GetPlayerPosition().z);
	//	m_graphics->GetBackgroundShader(3)->GetModels().at(0)->SetTranslation(m_graphics->GetPlayerPosition().x + 395, m_graphics->GetPlayerPosition().y, m_graphics->GetPlayerPosition().z);
	//	m_graphics->GetBackgroundShader(4)->GetModels().at(0)->SetTranslation(m_graphics->GetPlayerPosition().x, m_graphics->GetPlayerPosition().y - 100, m_graphics->GetPlayerPosition().z);
	//	m_graphics->GetBackgroundShader(5)->GetModels().at(0)->SetTranslation(m_graphics->GetPlayerPosition().x + 395, m_graphics->GetPlayerPosition().y - 100, m_graphics->GetPlayerPosition().z);
	//	m_graphics->GetBackgroundShader(6)->GetModels().at(0)->SetTranslation(m_graphics->GetPlayerPosition().x + 590, m_graphics->GetPlayerPosition().y - 100, m_graphics->GetPlayerPosition().z);
	//}
}

void TiledInterpreter::DestroyBackgrounds()
{
	for (int i = 6; i > -1; i--)
	{
		TextureShaderGeneralClass* tmp = m_graphics->GetBackgroundShader(i);
		if (tmp == nullptr)
			continue;

		tmp->ClearModels();
		tmp->Shutdown();
		delete tmp;
		tmp = 0;
	}

	m_graphics->ClearBackgrounds();
}
