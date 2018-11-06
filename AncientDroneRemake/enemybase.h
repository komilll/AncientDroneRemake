#pragma once

#include <d3d11.h>
#include <D3DX11.h>
#include <chrono>

#include "player.h"
#include "graphicsclass.h"

class EnemyBase
{
public:
	EnemyBase();
	EnemyBase(const EnemyBase&);
	~EnemyBase();

	virtual bool Init(GraphicsClass* graphicsClass, float width = 16, float height = 16, float translationX = 0.0f, float translationY = 0.0f);
	virtual void Update();
	virtual void Move(float x);
	virtual bool TouchedPlayer(Player* player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY);

private:
	//Function

public:
	//Variable

protected:
	GraphicsClass *m_graphics;
	ModelClass *m_model;
	float timer;
	__int64 lastTime;
	float speed = 0.5f;

	float frameMovementRight = 0.0f;
	float frameMovementUp = 0.0f;

	float gravity = 1.75f;
	bool useGravity = true;
};