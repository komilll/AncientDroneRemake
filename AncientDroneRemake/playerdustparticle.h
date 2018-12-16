#pragma once
#ifndef _PLAYER_DUST_PARTICLE_H_
#define _PLAYER_DUST_PARTICLE_H_

#include "graphicsclass.h"
#include "playeranimationstates.h"
#include <vector>
#include <chrono>

class PlayerDustParticle : IEAnimationObject
{
private:
	enum DustType {
		SIZE_1, SIZE_2, SIZE_3, SIZE_4, SIZE_5
	};

public:
	PlayerDustParticle();

	void SetNewAnimation(int newState) override;
	void Update();
	void SetNewAnimation(int newState, int index);
	bool Initialize(GraphicsClass* graphicsClass, int numberOfParticles);
	void SpawnParticle(float movevementDirection); //+move right; -move left

private:
	//Methods

private:
	//Helping variables
	GraphicsClass* m_graphics;

	//Use circular array to use particles pool
	std::vector<PlayerAnimationStates*> m_animations;
	std::vector<TextureShaderClass*> m_shaders;
	std::vector<ModelClass*> m_particles;
	std::vector<bool> m_particleInUse;
	std::vector<int> m_spawnTime;
	int m_particleIndex = 0;

	//Particles settings
	int m_liveTime = 500; //ms
	int m_liveTimeLeft = 0; //ms

	//Spawn timers
	__int64 m_lastSpawnTime = -1; //ms
	int m_spawnCooldown = 100; //ms
};

#endif // !_PLAYER_DUST_PARTICLE_H_