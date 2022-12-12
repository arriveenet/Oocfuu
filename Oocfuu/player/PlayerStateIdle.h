#pragma once
#include "PlayerState.h"

class PlayerStateIdle : public PlayerState {
public:
	PlayerStateIdle();

	void start(Player* _pPlayer) override;
	void update(PlayerStateContext* _pStateContext, Player* _pPlayer)override;
};