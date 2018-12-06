#include "tiledinterpreter.h"

TiledInterpreter::TiledInterpreter()
{

}

void TiledInterpreter::Initialize(GraphicsClass * graphicsClass, Player* player, GameManager* gameManager)
{
	m_graphics = graphicsClass;
	m_player = player;
	m_gameManager = gameManager;

	m_textureGeneral = new TextureShaderGeneralClass();
	m_textureGeneral->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND(), "Tile_1.dds");
	m_graphics->AddTextureShaderGeneral(m_textureGeneral);	

	ReadMapFile();
}

void TiledInterpreter::Import()
{
	for (int i = 0; i < MAP_WIDTH; i++)
	{
		for (int k = 0; k < MAP_HEIGHT; k++)
		{
			if (tab[i][k] >= TILE_MIN && tab[i][k] <= TILE_MAX)
				SpawnTile(i - firstTile, MAP_HEIGHT - k, tab[i][k]);
			else if (tab[i][k] == SPAWN_POINT)
				SpawnPlayer(TILE_SIZE * (i - firstTile) * 2, (MAP_HEIGHT - k) * TILE_SIZE * 2);
			else if (tab[i][k] >= WANDERER && tab[i][k] <= CROW)
				SpawnEnemy(i - firstTile, MAP_HEIGHT - k, tab[i][k]);
		}
	}
}

void TiledInterpreter::SpawnTile(int indexX, int indexY, int indexTile)
{	
	indexY -= 100;
	m_textureGeneral->AddModel(m_graphics->AddGroundModel(TILE_SIZE, TILE_SIZE, TILE_SIZE * indexX * 2, TILE_SIZE * indexY * 2));
}

void TiledInterpreter::ReadMapFile()
{
	ifstream iFile;
	//ofstream oFile;

	iFile.open("map.txt");
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
	iFile.open("mapWaypoints.txt");

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
	m_player->ChangePosition(posX, posY - TILE_SIZE * 100 * 2);
	m_player->Move();
}

void TiledInterpreter::SpawnEnemy(int indexX, int indexY, int indexEnemy)
{
	indexY -= 100;

	EnemyWanderer* wanderer = nullptr;
	EnemyArcher* archer = nullptr;
	EnemyFlying* flying = nullptr;

	switch (indexEnemy)
	{
		case WANDERER:
			wanderer = new EnemyWanderer();
			wanderer->Init(m_graphics, 16.0f, 16.0f, TILE_SIZE * indexX * 2, TILE_SIZE * indexY * 2, "mob_spikyback.dds");
			m_gameManager->AddNewEnemy(wanderer);
			break;
		case ARCHER:
			archer = new EnemyArcher();
			archer->Init(m_graphics, 12.0f, 12.0f, TILE_SIZE * indexX * 2, TILE_SIZE * indexY * 2);
			archer->SetPlayer(m_player);
			m_gameManager->AddNewEnemy(archer);
			break;
		case CROW:
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

			m_gameManager->AddNewEnemy(flying);
			break;
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
