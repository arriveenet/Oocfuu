#pragma once
#include "PlayerState.h"

class PlayerStateJump : public PlayerState {
public:
	void update(PlayerStateContext* _pStateContext, Player* _pPlayer);
};