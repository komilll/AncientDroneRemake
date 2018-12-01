#pragma once
#ifndef _TILED_INTERPRETER_H_
#define _TILED_INTERPRETER_H_

#include "graphicsclass.h"
#include "textureshadergeneralclass.h"
#include "player.h"
#include <fstream>
#include <istream>
#include <sstream>
#include <string>

class TiledInterpreter 
{
	const float TILE_SIZE = 16;

public:
	TiledInterpreter();
	void Initialize(GraphicsClass* graphicsClass, Player* player);
	void Import();

private:
	void SpawnTile(int indeX, int indexY, int indexTile);
	void ReadMapFile();
	void SpawnPlayer(float posX, float posY);

private:
	GraphicsClass* m_graphics;
	TextureShaderGeneralClass *m_textureGeneral;
	Player* m_player;
	static const int MAP_WIDTH = 100;
	static const int MAP_HEIGHT = 20;
	int tab[MAP_WIDTH][MAP_HEIGHT];
};

#endif // !_TILED_INTERPRETER_H_