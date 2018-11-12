#pragma once

#ifndef _ENEMY_ARCHER_ARROW_H_
#define _ENEMY_ARCHER_ARROW_H_

#include "movingobjectprototype.h"

class EnemyArcherArrow : public MovingObjectPrototype
{
	enum ArcherState {
		IDLE
	};
public:
	EnemyArcherArrow();

	bool Init(GraphicsClass* graphicsClass, float width = 16, float height = 16, float translationX = 0.0f, float translationY = 0.0f, CHAR* animationSheetName = "") override;
	void Update() override;
	void FixedUpdate() override;

};
#endif // !_ENEMY_ARCHER_ARROW_H_