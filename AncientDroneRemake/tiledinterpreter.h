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
#include "darksphere.h"
#include "levelfinish.h"
#include <fstream>
#include <istream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

static const float TILE_SIZE = 16;

class TiledInterpreter 
{
	//TILES
	static const int TILE_MIN = 1;
	static const int TILE_MAX = 8;
	/////////
	static const int END_POINT = 9;
	//ENEMIES
	static const int WANDERER = 10;
	static const int WANDERER_VERTICAL = 16;
	static const int ARCHER = 11;
	static const int CROW = 12;
	/////////
	static const int SPAWN_POINT = 13; //OTHER
	static const int WAYPOINT = 14; //OTHER
	//TRAPS
	static const int DARK_SPHERE = 15;
	/////////

public:
	TiledInterpreter();
	void Initialize(GraphicsClass* graphicsClass, Player* player, GameManager* gameManager);
	void Import(bool restart = false);
	void RestartLevel();
	void LoadNextLevel();
	void Update();

private:
	void SpawnTile(int indeX, int indexY, int indexTile);
	void ReadMapFile();
	void SpawnPlayer(float posX, float posY);
	void SpawnEnemy(int indexX, int indexY, int indexEnemy, bool restart = false);
	void SpawnDarkSphere(float posX, float posY, bool restart = false);
	void SpawnLevelFinish(float posX, float posY, bool restart = false);
	void FindFirstTileX();
	string GetTileName(int index);
	string GetLevelToLoad(int index);
	string GetWaypointToLoad(int index);
	void SpawnBackgrounds();
	void SpawnBackgroundsContinue();
	void DestroyBackgrounds();

private:
	GraphicsClass* m_graphics;
	std::vector<TextureShaderGeneralClass*>m_textureGeneral;
	GameManager* m_gameManager;
	Player* m_player;
	static const int MAP_WIDTH = 150;
	static const int MAP_HEIGHT = 20;
	int tab[MAP_WIDTH][MAP_HEIGHT];
	std::vector<D3DXVECTOR2> m_waypoints;
	int firstTile;

	std::vector<DarkSphere*> m_darkSpheres;

	int m_levelIndex = 0;

	//Restart params
	int m_wandererIndex = 0;
	int m_archerIndex = 0;
	int m_crowIndex = 0;

	string level_01 = "map01.txt";
	string levelWaypont_01 = "map01_waypoint.txt";
	string level_02 = "map02.txt";
	string levelWaypont_02 = "map02_waypoint.txt";
	string level_03 = "map03.txt";
	string levelWaypont_03 = "map03_waypoint.txt";

	__int64 m_startBackgroundSpawnTime = -1;
	bool m_backgroundSpawned = false;
};

#endif // !_TILED_INTERPRETER_H_