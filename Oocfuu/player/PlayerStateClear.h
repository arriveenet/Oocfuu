#pragma once
#include "PlayerState.h"
#include <glm/glm.hpp>

enum CLEAR_STEP {
	CLEAR_STEP_STOP,
	CLEAR_STEP_MOVE,
	CLEAR_STEP_STAY,
	CLEAR_STEP_MAX
};

class PlayerStateClear : public PlayerState {
	CLEAR_STEP m_step;
	bool m_lastScrollMax;

public:
	PlayerStateClear();

	void start(Player* _pPlayer) override;
	void update(PlayerStateContext* _pStateContext, Player* _pPlayer) override;
};