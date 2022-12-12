#pragma once
#include "PlayerState.h"

class PlayerStateRun : public PlayerState {
public:
	PlayerStateRun();

	void start(Player* _pPlayer) override;
	void update(PlayerStateContext* _pStateContext, Player* _pPlayer) override;
};