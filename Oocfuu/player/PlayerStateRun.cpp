#include "PlayerStateRun.h"
#include "PlayerStateJump.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "animation/Animation.h"

#include <glm/glm.hpp>

using namespace glm;

PlayerStateRun::PlayerStateRun()
	: PlayerState(PLAYER_STATE_RUN)
{
}

void PlayerStateRun::start(Player* _pPlayer)
{
}

void PlayerStateRun::update(PlayerStateContext* _pStateContext, Player* _pPlayer)
{
	if (Keyboard::m_pressed[PLAYER_KEY_RIGHT]) {
		if (g_pMouse->m_rightButton)
			_pPlayer->m_acceleration = 0.4f;
		else
			_pPlayer->m_acceleration = 0.2f;
		_pPlayer->m_speed.x += _pPlayer->m_acceleration;
		_pPlayer->m_flip = RECT_FLIP_NONE;
	}

	if (Keyboard::m_pressed[PLAYER_KEY_LEFT]) {
		if (g_pMouse->m_rightButton)
			_pPlayer->m_acceleration = 0.4f;
		else
			_pPlayer->m_acceleration = 0.2f;
		_pPlayer->m_speed.x -= _pPlayer->m_acceleration;
		_pPlayer->m_flip = RECT_FLIP_HORIZONTAL;
	}

	if (Keyboard::m_nowPressed[PLAYER_KEY_JUMP] && !_pPlayer->m_falling) {
		_pPlayer->m_animationController.setAnimation(ANIMATION_PLAYER_JUMP);
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
		_pPlayer->m_animationController.setAnimation(ANIMATION_PLAYER_IDLE);
		_pStateContext->setStete(new PlayerStateIdle);
	}
}
