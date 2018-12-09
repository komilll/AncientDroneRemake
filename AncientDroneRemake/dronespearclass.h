#pragma once
#ifndef _DRONE_SPEAR_H_
#define _DRONE_SPEAR_H_

#include "movingobjectprototype.h"

class DroneSpearClass : public MovingObjectPrototype
{
	enum SpearState {
		IDLE, DESTROY
	};

public:
	DroneSpearClass();

	//@Overriden
	bool Init(GraphicsClass* graphicsClass, float width = 16, float height = 16, float translationX = 0.0f, float translationY = 0.0f, CHAR* animationSheetName = "") override;
	void Update() override;
	void FixedUpdate() override; //Update function for physics called in fixed time
	void Move(float x) override;
	bool TouchedPlayer(Player* player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY) override;
	void SetNewAnimation(int newState) override;
	void PlayOneShotAnimation(int state, int previousState = -1) override;
	void HitedWall() override;
	void HeightTest(float mMinX, float mMaxX, float mMinY, float mMaxY, float gMinX, float gMaxX, float gMinY, float gMaxY, ModelClass* groundModel, bool heightTestPassed) override;

	//Local functions
	void Spawn();
	bool TouchedEnemy(MovingObjectPrototype *object);

private:
	void Destroy();

private:
	bool m_init = false;
	bool m_isMoving = false;
	float timeToDestroy = 3.5f;
	float currentTimeToDestroy = 0.0f;
};

#endif // !_DRONE_SPEAR_H_