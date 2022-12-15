#include "PlayerStateIdle.h"
#include "PlayerStateRun.h"
#include "PlayerStateJump.h"
#include "input/Keyboard.h"
#include "animation/Animation.h"
#include "sound/Sound.h"

#include <glm/glm.hpp>

using namespace glm;

//#define PLAYER_DEBUG_MODE

PlayerStateIdle::PlayerStateIdle()
	: PlayerState(PLAYER_STATE_IDLE)
{
}

void PlayerStateIdle::start(Player* _pPlayer)
{
}

void PlayerStateIdle::update(PlayerStateContext* _pStateContext, Player* _pPlayer)
{
#ifndef PLAYER_DEBUG_MODE
	if (Keyboard::m_pressed[PLAYER_KEY_RIGHT] || Keyboard::m_pressed[PLAYER_KEY_LEFT]) {
		_pPlayer->m_animeCtr.setAnimation(ANIMATION_PLAYER_RUN);
		_pStateContext->setStete(new PlayerStateRun);
	}

	if (Keyboard::m_nowPressed[PLAYER_KEY_JUMP]) {
		_pPlayer->m_animeCtr.setAnimation(ANIMATION_PLAYER_JUMP);
		_pStateContext->setStete(new PlayerStateJump);
	}
#else


	if (Keyboard::m_pressed[PLAYER_KEY_RIGHT]) {
		_pPlayer->m_speed.x += _pPlayer->m_acceleration;
	}

	if (Keyboard::m_pressed[PLAYER_KEY_LEFT]) {
		_pPlayer->m_speed.x -= _pPlayer->m_acceleration;
	}

	if (Keyboard::m_nowPressed[PLAYER_KEY_JUMP]
		&& (!_pPlayer->m_jumping)) {
		_pPlayer->m_jumping = _pPlayer->m_falling = true;
		g_pSound->play(SOUND_SE_HOI);
		_pPlayer->m_animeCtr.setAnimation(ANIMATION_PLAYER_JUMP);
	}

	if (_pPlayer->m_falling) {
		if (_pPlayer->m_jumping) {
			_pPlayer->m_speed.y = -4.5f;
			if (
				//(++m_jumpCount >= PLAYER_JUMP_COUNT_MAX)
				(!Keyboard::m_pressed[PLAYER_KEY_JUMP])
				)
				_pPlayer->m_jumping = false;
		}
		_pPlayer->m_speed.y += .4f;
	}

	// ‘¬“x‚É–€ŽC‚Ì‰e‹¿‚ð—^‚¦‚é(ŒÅ’è¬”“_)
	{
		static int one = 0x100;
		ivec2 fixed = _pPlayer->m_speed * (float)one;
		fixed = fixed * 0xe8 / one;
		_pPlayer->m_speed = (vec2)fixed / (float)one;
	}

	// ‘¬“x‚Ì‰e‹¿‚ðÀ•W‚É—^‚¦‚é
	_pPlayer->m_position += _pPlayer->m_speed;

	// Animation settings
	switch (_pPlayer->m_animeCtr.m_animation) {
	case ANIMATION_PLAYER_IDLE:
		if (Keyboard::m_pressed[PLAYER_KEY_LEFT] || Keyboard::m_pressed[PLAYER_KEY_RIGHT])
			_pPlayer->m_animeCtr.setAnimation(ANIMATION_PLAYER_RUN);

		if (Keyboard::m_pressed[PLAYER_KEY_LEFT])
			_pPlayer->m_flip = RECT_FLIP_HORIZONTAL;

		if (Keyboard::m_pressed[PLAYER_KEY_RIGHT])
			_pPlayer->m_flip = RECT_FLIP_NONE;

		break;
	case ANIMATION_PLAYER_RUN:
		if (_pPlayer->m_speed.x < 0)
			_pPlayer->m_flip = RECT_FLIP_HORIZONTAL;
		if (_pPlayer->m_speed.x > 0)
			_pPlayer->m_flip = RECT_FLIP_NONE;

		if (length(_pPlayer->m_speed) <= 0)
			_pPlayer->m_animeCtr.setAnimation(ANIMATION_PLAYER_IDLE);
		break;

	case ANIMATION_PLAYER_JUMP:
		if (!_pPlayer->m_falling)
			_pPlayer->m_animeCtr.setAnimation(ANIMATION_PLAYER_IDLE);
		break;
	}


#endif
}
