#pragma once
#include "PlayerState.h"

class PlayerStateRun : public PlayerState {
public:
	PlayerStateRun();

	void update(PlayerStateContext* _pStateContext, Player* _pPlayer);
};