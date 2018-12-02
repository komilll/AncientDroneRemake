#include "tiledinterpreter.h"

TiledInterpreter::TiledInterpreter()
{

}

void TiledInterpreter::Initialize(GraphicsClass * graphicsClass, Player* player)
{
	m_graphics = graphicsClass;
	m_player = player;

	m_textureGeneral = new TextureShaderGeneralClass();
	m_textureGeneral->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND(), "Tile_1.dds");
	m_graphics->AddTextureShaderGeneral(m_textureGeneral);	

	ReadMapFile();
}

void TiledInterpreter::Import()
{
	const int SPAWN_POINT = 9;
	const int TILE_MIN = 1;
	const int TILE_MAX = 8;

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
				SpawnPlayer(TILE_SIZE * (i - firstTile) * 2, /*(MAP_HEIGHT - k) * TILE_SIZE * 2*/ 0);
		}
	}
}

void TiledInterpreter::SpawnTile(int indeX, int indexY, int indexTile)
{	
	indexY -= 5;
	m_textureGeneral->AddModel(m_graphics->AddGroundModel(TILE_SIZE, TILE_SIZE, TILE_SIZE * indeX * 2, TILE_SIZE * indexY * 2));
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
	m_player->ChangePosition(posX, posY);
	m_player->Move();
}