#pragma once
#include "PlayerState.h"

enum  PLAYERSTATEGOAL {
	PLAYERSTATEGOAL_FALL,
	PLAYERSTATEGOAL_FLIP,
	PLAYERSTATEGOAL_MOVE,
	PLAYERSTATEGOAL_WAIT,
	PLAYERSTATEGOAL_MAX
};

class PlayerStateGoal : public PlayerState
{
	PLAYERSTATEGOAL m_state;
	unsigned int m_counter;

public:
	PlayerStateGoal();

	void start(Player* _pPlayer) override;
	void update(PlayerStateContext* _pStateContext, Player* _pPlayer) override;
};