#pragma once

#include "enemybase.h"

class EnemyWanderer : public EnemyBase
{
	enum EnemyState {
		IDLE, MOVING, ATTACKING
	};

public:
	EnemyWanderer();
	EnemyWanderer(const EnemyWanderer&);
	~EnemyWanderer();

	bool Init(GraphicsClass* graphicsClass, float width = 16, float height = 16, float translationX = 0.0f, float translationY = 0.0f, CHAR* animationSheetName = "") override;
	void Update() override;
	void FixedUpdate() override; //Update function for physics called in fixed time
	bool TouchedPlayer(Player * player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY) override;
	void Move(float x) override;
	void HitedWall() override;

	void ForceVerticalMovement();

private:
	int m_wanderTimer = 0;
	bool m_moveOnlyVertically = false;
};