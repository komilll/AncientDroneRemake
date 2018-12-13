#pragma once
#ifndef _DARK_SPHERE_H_
#define _DARK_SPHERE_H_

#include "graphicsclass.h"
#include "player.h"
#include <chrono>

class DarkSphere 
{
public:
	DarkSphere();

	bool Init(GraphicsClass* graphicsClass, float translationX, float translationY);
	void Update();
	void FixedUpdate();
	bool TouchedPlayer(Player* player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY);
	void Shutdown();

private:
	void StartUsing();

private:
	GraphicsClass* m_graphics;
	ModelClass* m_modelBase;
	ModelClass* m_modelLerped;
	TextureShaderGeneralClass* m_shader;

	float m_currentScale = 1.0f;
	float m_maxScale = 5.0f;
	float m_minScale = 0.5f;
	float m_maxRandom = 0.15f;

	float m_cooldown = 3000;
	float m_duration = 1000;
	float m_sphereTimer = 0;
	bool m_isActive = false;

	float timer;
	__int64 lastTime;
};


#endif // !_DARK_SPHERE_H_