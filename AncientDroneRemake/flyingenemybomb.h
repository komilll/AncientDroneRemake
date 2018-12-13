#pragma once

#include "graphicsclass.h"
#include "player.h"

class FlyingEnemyBomb
{
public:
	///<summary>
	///Radius; How long explosion lasts; Maximum scale in relation to start radius which is 1.0
	///<summary>
	FlyingEnemyBomb(GraphicsClass* graphicsClass, float radius, float explosionTime, float maxScale = 2.0f, float gravityScale = 2.0f);

	void Update();
	ModelClass* GetModel();
	bool TouchedPlayer(Player * player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY);
	void Init(float spawnPosX, float spawnPosY);
	void Shutdown();

private:
	GraphicsClass* m_graphics;
	ModelClass* m_model;

	bool m_isExploding = false;
	bool m_used = false;
	bool m_init = false;
	bool m_damaged = false;

	float m_currentScale;
	float m_scalePerFrame;

	float m_explosionRadius;
	float m_explosionTime;
	float m_explosionMinScale;
	float m_explosionMaxScale;
	float m_gravityScale;

};