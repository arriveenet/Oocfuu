#pragma once
#include "PlayerState.h"
#include <glm/glm.hpp>

class PlayerStateClear : public PlayerState {
	glm::vec2 m_lastSpeed;
public:
	PlayerStateClear();

	void start(Player* _pPlayer) override;
	void update(PlayerStateContext* _pStateContext, Player* _pPlayer) override;
};