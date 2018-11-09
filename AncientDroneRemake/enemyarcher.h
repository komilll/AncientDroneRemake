#pragma once

#include "enemybase.h"
#include "enemyarcherarrow.h"
#include "player.h"

class EnemyArcher : public EnemyBase
{
public:
	EnemyArcher();
	EnemyArcher(const EnemyArcher&);
	~EnemyArcher();

	bool Init(GraphicsClass* graphicsClass, float width = 16, float height = 16, float translationX = 0.0f, float translationY = 0.0f) override;
	void Update() override;
	void FixedUpdate() override;

	void SetPlayer(Player* player);

private:
	Player* player;	

	//Arrows spawning
	std::vector<EnemyArcherArrow*> m_arrow;
	float m_arrowCooldownTime = 2.0f;
	float m_arrowCooldownCurrent = 0.0f;
	int m_arrowIndex = 0;
};