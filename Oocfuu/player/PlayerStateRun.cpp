#include "PlayerStateRun.h"
#include "PlayerStateJump.h"
#include "Player.h"
#include "../Keyboard.h"

#include <glm/glm.hpp>

using namespace glm;

void PlayerStateRun::update(PlayerStateContext* _pStateContext, Player* _pPlayer)
{
	if (Keyboard::m_pressed[PLAYER_KEY_RIGHT]) {

	}

	if (Keyboard::m_pressed[PLAYER_KEY_LEFT]) {

	}

	if (Keyboard::m_pressed[PLAYER_KEY_JUMP]) {
		_pStateContext->setStete(new PlayerStateJump);
	}
}
