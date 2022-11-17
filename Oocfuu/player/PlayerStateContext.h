#pragma once
#include "PlayerState.h"
#include "Player.h"

class PlayerState;
class Player;

class PlayerStateContext
{
	PlayerState* m_pState;

public:
	PlayerStateContext();
	virtual ~PlayerStateContext();

	void setStete(PlayerState* _pState);
	void update(Player* _pPlayer);
};

