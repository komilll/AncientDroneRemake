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

	//Local functions
	bool Attack();
	void SetDestination(float destX, float destY);
	void CheckSpearsDamage(MovingObjectPrototype* object);
	float GetDroneEnergyProgress();
	void DroneOutsideScreen();
	void FreeDroneFromPlayer();
	void CallDroneToPlayer();

private:
	//SPEAR
	std::vector<DroneSpearClass*> m_spears;
	int m_spearIndex = 0;

	float m_destX, m_destY;
	float m_epsilon = 0.1f;
	float m_epsilonDistance = 0.3f;
	float m_attackCooldown = 1.0f;
	float m_attackCooldownCurrent = 0.0f;
	int m_energyMax = 100;
	int m_energyLeft = m_energyMax;
	int m_bulletEnergyCost = 25;
	__int64 m_lastAttackTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	int m_timeToRecoverEnergy = 2000; //ms

	//Movement
	bool m_droneOnPlayer = false;
	bool m_droneForceToPlayer = false;
	float m_distToPlayerX = -7.5f;
	float m_distToPlayerY = 11.0f;
	float m_boundsScreenX = 141;
	float m_boundsScreenY = 106;
};
#endif // !_DRONE_CONTORLLER_H_