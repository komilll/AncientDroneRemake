#pragma once

#include "enemybase.h"
#include "flyingenemybomb.h"
#include "player.h"

class EnemyFlying : public EnemyBase
{
public:
	EnemyFlying();
	EnemyFlying(const EnemyFlying&);
	~EnemyFlying();

	bool Init(GraphicsClass* graphicsClass, float width = 16, float height = 16, float translationX = 0.0f, float translationY = 0.0f) override;
	void Update() override;
	void Move(float x) override;

	void SetPlayer(Player* player);
	void SetWaypoints(D3DXVECTOR2 waypoint1, D3DXVECTOR2 waypoint2);

private:
	D3DXVECTOR2 leftWaypoint;
	D3DXVECTOR2 rightWaypoint;

	FlyingEnemyBomb* m_bomb;
	Player* player;
};