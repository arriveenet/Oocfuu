#pragma once
#include "PlayerState.h"

class PlayerStateDie : public PlayerState
{
	unsigned int m_counter;

public:
	PlayerStateDie();
	void update(PlayerStateContext* _pStateContext, Player* _pPlayer);
};