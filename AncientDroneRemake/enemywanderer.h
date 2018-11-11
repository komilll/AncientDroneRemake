#pragma once

#include "enemybase.h"

class EnemyWanderer : public EnemyBase
{
public:
	EnemyWanderer();
	EnemyWanderer(const EnemyWanderer&);
	~EnemyWanderer();

	bool Init(GraphicsClass* graphicsClass, float width = 16, float height = 16, float translationX = 0.0f, float translationY = 0.0f, CHAR* animationSheetName = "") override;
	void Update() override;
	void Move(float x) override;

};