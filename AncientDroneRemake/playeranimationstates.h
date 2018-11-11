#pragma once

#ifndef _H_PLAYERANIMATIONSTATES
#define _H_PLAYERANIMATIONSTATES


#include <d3d11.h>
#include <D3DX11.h>

class PlayerAnimationStates
{
public:
	PlayerAnimationStates(int numberOfStates);

	int GetCurrentState();
	void SetState(int newState);
	void PrepareAnimationPose(int state_, int animationPoseIndex);
	int GetAnimationIndex();

private:
	int state;
	int *m_animationPoseIndex;
	int numberOfStates;
};



#endif // !_H_PLAYERANIMATIONSTATES