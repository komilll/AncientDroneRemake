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
	int firstTile = 0;

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

	//for (int i = 0; i < x; i++)
	//{
	//	for (int k = 0; k < y; k++)
	//	{
	//		if (tab[i][k] > 0)
	//		{
	//			//char* toWrite = "[" + (char*)i + "][" + (char*)k + "] = " + (char*)tab[i][k];				
	//			oFile.write("[", 1);
	//			//oFile.write((char*)i, 1);
	//			oFile.write("][", 2);
	//			//oFile.write((char*)k, 1);
	//			oFile.write("] = ", 4);
	//			//oFile.write((char*)tab[i][k], 1);
	//			oFile.write("\n", 1);
	//		}
	//	}
	//}

	iFile.close();
	//oFile.close();
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
			//enemyFlying = new EnemyFlying();
			//if (enemyFlying == nullptr)
			//	return false;

			//if (!enemyFlying->Init(graphicsClass, 16, 16))
			//	return false;

			//enemyFlying->SetPlayer(player);
			//enemyFlying->SetWaypoints(D3DXVECTOR2(-120.0f, 0.0f), D3DXVECTOR2(120.0f, 0.0f));
			break;
	}

}