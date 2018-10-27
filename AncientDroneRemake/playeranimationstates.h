#pragma once

#ifndef _H_PLAYERANIMATIONSTATES
#define _H_PLAYERANIMATIONSTATES


#include <d3d11.h>
#include <D3DX11.h>

static class PlayerAnimationStates
{
	enum State{
		IDLE,
		MOVING,
		COMMAND,
		FALLING
	};
};



#endif // !_H_PLAYERANIMATIONSTATES