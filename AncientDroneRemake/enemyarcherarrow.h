#pragma once

#ifndef _ENEMY_ARCHER_ARROW_H_
#define _ENEMY_ARCHER_ARROW_H_

#include "movingobjectprototype.h"

class EnemyArcherArrow : public MovingObjectPrototype
{
	enum ArcherState {
		IDLE, DESTROY
	};
public:
	EnemyArcherArrow();

	bool Init(GraphicsClass* graphicsClass, float width = 16, float height = 16, float translationX = 0.0f, float translationY = 0.0f, CHAR* animationSheetName = "") override;
	void Update() override;
	void FixedUpdate() override;
	bool IsActive();
	void SetDirection(bool movingRight, float posX = -1.0f, float posY = -1.0f);
	void DestroyArrow();
	void HitedWall() override;

private:
	bool m_init = false;
	bool m_isMoving = false;
	float timeToDestroy = 2.0f;
	float currentTimeToDestroy = 0.0f;
};
#endif // !_ENEMY_ARCHER_ARROW_H_