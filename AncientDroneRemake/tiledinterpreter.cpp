#include "tiledinterpreter.h"

TiledInterpreter::TiledInterpreter()
{

}

void TiledInterpreter::Initialize(GraphicsClass * graphicsClass)
{
	m_graphics = graphicsClass;

	m_textureGeneral = new TextureShaderGeneralClass();
	m_textureGeneral->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND(), "Tile_1.dds");
	m_graphics->AddTextureShaderGeneral(m_textureGeneral);
}

void TiledInterpreter::Import()
{
	const int x = 4;
	const int y = 1;
	int tab[x][y] = { {1}, {2}, {3}, {4} };
	
	const int SPAWN_POINT = 8;
	const int TILE_MIN = 1;
	const int TILE_MAX = 8;

	int firstTile = 0;

	//Find most-left tile to use as start point
	for (int i = 0; i < x; i++)
	{
		for (int k = 0; k < y; k++)
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

	for (int i = 0; i < x; i++)
	{
		for (int k = 0; k < y; k++)
		{
			if (tab[i][k] >= TILE_MIN && tab[i][k] <= TILE_MAX)
				SpawnTile(i - firstTile, k, tab[i][k]);
		}
	}
}

void TiledInterpreter::SpawnTile(int indeX, int indexY, int indexTile)
{	
	indexY = -3;
	m_textureGeneral->AddModel(m_graphics->AddGroundModel(TILE_SIZE, TILE_SIZE, TILE_SIZE * indeX * 2, TILE_SIZE * indexY * 2));
}
