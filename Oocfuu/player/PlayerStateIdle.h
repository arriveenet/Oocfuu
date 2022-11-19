#pragma once
#include "PlayerState.h"

class PlayerStateIdle : public PlayerState {
public:
	PlayerStateIdle();

	void update(PlayerStateContext* _pStateContext, Player* _pPlayer);
};