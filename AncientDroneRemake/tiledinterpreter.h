#pragma once
#ifndef _TILED_INTERPRETER_H_
#define _TILED_INTERPRETER_H_

#include "graphicsclass.h"
#include "textureshadergeneralclass.h"

class TiledInterpreter 
{
	const float TILE_SIZE = 16;

public:
	TiledInterpreter();
	void Initialize(GraphicsClass* graphicsClass);
	void Import();

private:
	void SpawnTile(int indeX, int indexY, int indexTile);


private:
	GraphicsClass* m_graphics;
	TextureShaderGeneralClass *m_textureGeneral;

};

#endif // !_TILED_INTERPRETER_H_