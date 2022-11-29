#pragma once
#include "Player.h"
#include "PlayerStateContext.h"

enum PLAYER_STATE {
	PLAYER_STATE_IDLE,
	PLAYER_STATE_RUN,
	PLAYER_STATE_JUMP,
	PLAYER_STATE_MAX
};

class Player;
class PlayerStateContext;

class PlayerState {
	PLAYER_STATE m_state;

public:
	PlayerState(PLAYER_STATE _state)
		: m_state(_state)
		{};

	virtual void update(PlayerStateContext* _pStateContext, Player* _pPlayer) = 0;
	PLAYER_STATE getState() const { return m_state; };
};