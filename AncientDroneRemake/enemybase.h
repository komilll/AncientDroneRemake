#pragma once

#include "movingobjectprototype.h"

class EnemyBase : public MovingObjectPrototype
{
public:
	EnemyBase();
	EnemyBase(const EnemyBase&);
	~EnemyBase();

	bool Init(GraphicsClass* graphicsClass, float width = 16, float height = 16, float translationX = 0.0f, float translationY = 0.0f, CHAR* animationSheetName = "") override;
	void Update() override;
	void FixedUpdate() override; //Update function for physics called in fixed time
	void Move(float x) override;
	bool TouchedPlayer(Player* player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY) override;

	void SetNewAnimation(int newState) override final;
	void PlayOneShotAnimation(int state, int previousState = -1) override final;
	virtual void RestartEnemy() final;

private:
	//Function

public:
	//Variable

private:
	//Variable
};