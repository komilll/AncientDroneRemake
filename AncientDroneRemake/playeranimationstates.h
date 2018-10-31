#pragma once

#ifndef _H_PLAYERANIMATIONSTATES
#define _H_PLAYERANIMATIONSTATES


#include <d3d11.h>
#include <D3DX11.h>

enum StatePlayer {
	IDLE,
	MOVING,
	COMMAND,
	FALLING
};

static class PlayerAnimationStates
{


public:
	StatePlayer GetCurrentState();
	void SetState(StatePlayer newState);
	void PrepareAnimationPose(StatePlayer state_, int animationPoseIndex);
	int GetAnimationIndex();

private:
	StatePlayer state;
	int m_animationPoseIndex[4];
};



#endif // !_H_PLAYERANIMATIONSTATES