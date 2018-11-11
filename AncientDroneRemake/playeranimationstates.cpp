#include "playeranimationstates.h"

PlayerAnimationStates::PlayerAnimationStates(int numberOfStates_)
{
	numberOfStates = numberOfStates_;
	m_animationPoseIndex = new int[numberOfStates_];
}

int PlayerAnimationStates::GetCurrentState()
{
	return state;
}

void PlayerAnimationStates::SetState(int newState)
{
	state = newState;
}

void PlayerAnimationStates::PrepareAnimationPose(int state_, int animationPoseIndex)
{
	if (state_ >= numberOfStates)
		return;

	m_animationPoseIndex[state_] = animationPoseIndex;
}

int PlayerAnimationStates::GetAnimationIndex()
{
	return m_animationPoseIndex[state];
}
