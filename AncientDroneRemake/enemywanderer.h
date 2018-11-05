#pragma once

#include "enemybase.h"

class EnemyWanderer : public EnemyBase
{
public:
	EnemyWanderer();
	EnemyWanderer(const EnemyWanderer&);
	~EnemyWanderer();

	bool Init(GraphicsClass* graphicsClass) override;
	void Update() override;
	void Move(float x) override;

};