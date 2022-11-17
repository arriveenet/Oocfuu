#pragma once
#include "PlayerState.h"

class PlayerStateRun : public PlayerState {
public:
	void update(PlayerStateContext* _pStateContext, Player* _pPlayer);
};