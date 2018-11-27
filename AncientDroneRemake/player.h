#pragma once

#include <chrono>
#include <iostream>
#include <sstream>

#include "inputclass.h"
#include "modelclass.h"
#include "d3dclass.h"
#include "graphicsclass.h"
#include "playeranimationstates.h"
#include "IEanimationobject.h"

#pragma region KEYBOARD_KEYS

//	*VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
//	* 0x40 : unassigned
//	* VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)

#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A
#pragma endregion

///<summary>Class for on-screen character input and gameplay values</summary>
class Player : IEAnimationObject
{
	enum StatePlayer {
		IDLE,
		MOVING,
		COMMAND,
		FALLING
	};

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
	void DealDamage(int dmg);
	Bounds GetBounds();
	float GetHealthProgress();

private:
	void PlayerDeath();
	void SetNewAnimation(int newState) override final; //IE
	void OnDestroy();

private:	
	//External Classes
	InputClass *m_input; //Class referenced from SystemClass; Singleton
	D3DClass *m_d3d;
	GraphicsClass *m_graphics;
	//////////////////
	ModelClass *m_playerModel;
	TextureShaderClass* m_shaderClass;
	PlayerAnimationStates *m_playerAnimation;
	
	//Input Options
	int btn_moveRight = VK_D;
	int btn_moveLeft = VK_A;
	int btn_moveUp = VK_UP;
	int btn_moveDown = VK_DOWN;
	int btn_jump = VK_SPACE;
	///////////////

	//Movement
	int movementRight;
	int movementUp;
	bool isGround = false;
	int jumpTicks = 8;
	int currentJumpTicks = 0;
	float jumpTickHeight = 3.75f;
	int idleTime = 0;
	int timeToSetIdleState = 15;

	//Physics
	float gravity = 1.75f;
	float timer;
	__int64 lastTime;

	//Gameplay
	int maxHealth = 10;
	int health = maxHealth;
	bool invincible = false;
};