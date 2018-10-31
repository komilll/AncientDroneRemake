#pragma once

#include <d3d11.h>
#include <D3DX11.h>
#include <chrono>

#include "graphicsclass.h"

class EnemyBase
{
public:
	EnemyBase();
	EnemyBase(const EnemyBase&);
	~EnemyBase();

	bool Init(GraphicsClass* graphicsClass);
	virtual void Update();
	void Move(float x);

private:
	//Function

public:
	//Variable

private:
	GraphicsClass *m_graphics;
	ModelClass *m_model;
	float gravity = 1.75f;
	float timer;
	__int64 lastTime;
};