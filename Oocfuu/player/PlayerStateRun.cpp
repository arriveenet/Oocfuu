#include "PlayerStateRun.h"
#include "PlayerStateJump.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "../Keyboard.h"
#include "../animation/Animation.h"

#include <glm/glm.hpp>

using namespace glm;

PlayerStateRun::PlayerStateRun()
	: PlayerState(PLAYER_STATE_RUN)
{
}

void PlayerStateRun::update(PlayerStateContext* _pStateContext, Player* _pPlayer)
{
	static float acceleration = 0.2f;
	if (Keyboard::m_pressed[PLAYER_KEY_RIGHT]) {
		_pPlayer->m_speed.x += acceleration;
		_pPlayer->m_flip = RECT_FLIP_NONE;
	}

	if (Keyboard::m_pressed[PLAYER_KEY_LEFT]) {
		_pPlayer->m_speed.x -= acceleration;
		_pPlayer->m_flip = RECT_FLIP_HORIZONTAL;
	}

	if (Keyboard::m_nowPressed[PLAYER_KEY_JUMP]) {
		_pPlayer->m_animeCtr.setAnimation(ANIMATION_PLAYER_JUMP);
		_pStateContext->setStete(new PlayerStateJump);
		return;
	}

	{
		static int one = 0x100;
		ivec2 fixed = _pPlayer->m_speed * (float)one;
		fixed = fixed * 0xe8 / one;
		_pPlayer->m_speed = (vec2)fixed / (float)one;
	}
	//_pPlayer->m_position += _pPlayer->m_speed;

	if (glm::length(_pPlayer->m_speed.x) <= 0) {
		_pPlayer->m_animeCtr.setAnimation(ANIMATION_PLAYER_IDLE);
		_pStateContext->setStete(new PlayerStateIdle);
	}
}
