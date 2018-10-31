#pragma once

#include "enemybase.h"

class EnemyWanderer : public EnemyBase
{
public:
	EnemyWanderer();
	EnemyWanderer(const EnemyWanderer&);
	~EnemyWanderer();
};