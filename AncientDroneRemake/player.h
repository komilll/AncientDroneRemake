#pragma once

#include <chrono>
#include <iostream>
#include <sstream>

#include "inputclass.h"
#include "modelclass.h"
#include "d3dclass.h"
#include "graphicsclass.h"
#include "playeranimationstates.h"

///<summary>Class for on-screen character input and gameplay values</summary>
class Player
{
public:
	Player();
	Player(InputClass *inputClass, D3DClass *d3d, GraphicsClass *graphicsClass);
	Player(const Player&);
	~Player();

	bool Initialize();
	bool Initialize(InputClass *inputClass);
	void Update();
	void FixedUpdate();
	void Move();

private:	
	//External Classes
	InputClass *m_input; //Class referenced from SystemClass; Singleton
	D3DClass *m_d3d;
	GraphicsClass *m_graphics;
	//////////////////
	ModelClass *m_playerModel;
	PlayerAnimationStates *m_playerAnimation;
	
	void SetNewAnimation(State newState);
	void OnDestroy();

	//Input Options
	int btn_moveRight = VK_RIGHT;
	int btn_moveLeft = VK_LEFT;
	int btn_moveUp = VK_UP;
	int btn_moveDown = VK_DOWN;
	int btn_jump = VK_SPACE;
	///////////////

	//Movement
	int movementRight;
	int movementUp;
	bool isGround = false;
	int jumpTicks = 40;
	int currentJumpTicks = 0;
	float jumpTickHeight = 0.75f;
	int idleTime = 0;
	int timeToSetIdleState = 15;

	//Physics
	float gravity = 1.75f;
	float timer;
	__int64 lastTime;
};