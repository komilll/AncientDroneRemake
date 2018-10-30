#pragma once

#ifndef _H_PLAYERANIMATIONSTATES
#define _H_PLAYERANIMATIONSTATES


#include <d3d11.h>
#include <D3DX11.h>

enum State {
	IDLE,
	MOVING,
	COMMAND,
	FALLING
};

static class PlayerAnimationStates
{


public:
	State GetCurrentState();
	void SetState(State newState);
	void PrepareAnimationPose(State state_, int animationPoseIndex);
	int GetAnimationIndex();

private:
	State state;
	int m_animationPoseIndex[4];
};



#endif // !_H_PLAYERANIMATIONSTATES