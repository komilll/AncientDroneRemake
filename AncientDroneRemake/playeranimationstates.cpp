#include "playeranimationstates.h"

StatePlayer PlayerAnimationStates::GetCurrentState()
{
	return state;
}

void PlayerAnimationStates::SetState(StatePlayer newState)
{
	state = newState;
}

void PlayerAnimationStates::PrepareAnimationPose(StatePlayer state_, int animationPoseIndex)
{
	m_animationPoseIndex[state_] = animationPoseIndex;
}

int PlayerAnimationStates::GetAnimationIndex()
{
	return m_animationPoseIndex[state];
}
