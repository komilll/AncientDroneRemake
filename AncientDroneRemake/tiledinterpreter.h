#pragma once
#ifndef _TILED_INTERPRETER_H_
#define _TILED_INTERPRETER_H_

#include "graphicsclass.h"
#include "textureshadergeneralclass.h"
#include "player.h"
#include "gamemanager.h"
#include "enemywanderer.h"
#include "enemyflying.h"
#include "enemyarcher.h"
#include <fstream>
#include <istream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

class TiledInterpreter 
{
	const float TILE_SIZE = 16;
	//TILES
	static const int TILE_MIN = 1;
	static const int TILE_MAX = 8;
	/////////
	static const int END_POINT = 9;
	//ENEMIES
	static const int WANDERER = 10;
	static const int ARCHER = 11;
	static const int CROW = 12;
	/////////
	static const int SPAWN_POINT = 13; //OTHER
	static const int WAYPOINT = 14; //OTHER

public:
	TiledInterpreter();
	void Initialize(GraphicsClass* graphicsClass, Player* player, GameManager* gameManager);
	void Import();

private:
	void SpawnTile(int indeX, int indexY, int indexTile);
	void ReadMapFile();
	void SpawnPlayer(float posX, float posY);
	void SpawnEnemy(int indexX, int indexY, int indexEnemy);
	void FindFirstTileX();

private:
	GraphicsClass* m_graphics;
	TextureShaderGeneralClass *m_textureGeneral;
	GameManager* m_gameManager;
	Player* m_player;
	static const int MAP_WIDTH = 100;
	static const int MAP_HEIGHT = 20;
	int tab[MAP_WIDTH][MAP_HEIGHT];
	std::vector<D3DXVECTOR2> m_waypoints;
	int firstTile;
};

#endif // !_TILED_INTERPRETER_H_