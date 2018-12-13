#pragma once
#ifndef _LEVEL_FINISH_H
#define _LEVEL_FINISH_H

#include "graphicsclass.h"
#include "player.h"
#include <chrono>

class LevelFinish 
{
public:
	LevelFinish();

	bool Init(GraphicsClass* graphicsClass, float translationX, float translationY);
	void Update();
	void FixedUpdate();
	bool TouchedPlayer(Player* player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY);
	bool ReadyForNextLevel();
	void Shutdown();

private:
	GraphicsClass* m_graphics;
	ModelClass* m_model;
	TextureShaderGeneralClass* m_shader;

	float m_currentScale = 5.0f;
	float m_maxRandom = 0.15f;

	bool m_nextLevelReady = false;

	float timer;
	__int64 lastTime;
};

#endif // !_LEVEL_FINISH_H