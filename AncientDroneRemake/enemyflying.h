#pragma once

#include "enemybase.h"
#include "flyingenemybomb.h"
#include "player.h"

class EnemyFlying : public EnemyBase
{
	enum EnemyState {
		IDLE, MOVING, ATTACKING
	};

public:
	EnemyFlying();
	EnemyFlying(const EnemyFlying&);
	~EnemyFlying();

	bool Init(GraphicsClass* graphicsClass, float width = 16, float height = 16, float translationX = 0.0f, float translationY = 0.0f, CHAR* animationSheetName = "") override;
	void Update() override;
	void FixedUpdate() override;
	void Move(float x) override;

	void SetPlayer(Player* player);
	void SetWaypoints(D3DXVECTOR2 waypoint1, D3DXVECTOR2 waypoint2);

private:
	D3DXVECTOR2 leftWaypoint;
	D3DXVECTOR2 rightWaypoint;

	Player* player;

	//Bombs
	std::vector<FlyingEnemyBomb*> m_bomb;
	float m_bombCooldownTime = 2.0f;
	float m_bombCooldownCurrent = 0.0f;
	int m_bombIndex = 0;

};