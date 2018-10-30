#include "playeranimationstates.h"

State PlayerAnimationStates::GetCurrentState()
{
	return state;
}

void PlayerAnimationStates::SetState(State newState)
{
	state = newState;
}

void PlayerAnimationStates::PrepareAnimationPose(State state_, int animationPoseIndex)
{
	m_animationPoseIndex[state_] = animationPoseIndex;
}

int PlayerAnimationStates::GetAnimationIndex()
{
	return m_animationPoseIndex[state];
}
