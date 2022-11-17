#pragma once
#include "Player.h"
#include "PlayerStateContext.h"

class Player;
class PlayerStateContext;

class PlayerState {
public:
	virtual void update(PlayerStateContext* _pStateContext, Player* _pPlayer) = 0;
};