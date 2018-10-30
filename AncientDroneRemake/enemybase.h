#pragma once

#include <d3d11.h>
#include <D3DX11.h>

class EnemyBase
{
public:
	EnemyBase();
	EnemyBase(const EnemyBase&);
	~EnemyBase();

	void Init();
	void Update();
};