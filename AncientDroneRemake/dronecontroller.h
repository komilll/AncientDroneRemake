#pragma once
#ifndef _DRONE_CONTORLLER_H_
#define _DRONE_CONTORLLER_H_

#include "movingobjectprototype.h"
#include "dronespearclass.h"

class DroneController : public MovingObjectPrototype
{
	enum DroneState {
		IDLE, ATTACKING
	};

public:
	DroneController();

	//@Overriden
	bool Init(GraphicsClass* graphicsClass, float width = 16, float height = 16, float translationX = 0.0f, float translationY = 0.0f, CHAR* animationSheetName = "") override;
	void Update() override;
	void FixedUpdate() override; //Update function for physics called in fixed time
	void Move(float x) override;
	bool TouchedPlayer(Player* player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY) override;
	void SetNewAnimation(int newState) override;
	void PlayOneShotAnimation(int state, int previousState = -1) override;
	void HitedWall() override;

	bool Attack();

private:
	std::vector<DroneSpearClass*> m_spears;
};
#endif // !_DRONE_CONTORLLER_H_