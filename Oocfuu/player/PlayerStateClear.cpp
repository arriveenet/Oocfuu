#include "PlayerStateClear.h"
#include "App.h"
#include "animation/Animation.h"
#include "world/Course.h"
#include "world/Part.h"
#include "enemy/EnemyManager.h"
#include "sound/Sound.h"
#include <vector>

using namespace std;
using namespace glm;

namespace {
	MESSAGE messages[] =
	{
		{"THANK YOU OOCFUU!", vec2(64, 80), 0},
		{"BUT OUR PRINCESS IS IN\n\nANOTHER CASTLE!", vec2(40, 112), 96}
	};

	const int messageSize = sizeof(messages) / sizeof(MESSAGE);
}

PlayerStateClear::PlayerStateClear()
	: PlayerState(PLAYER_STATE_CLEAR)
	, m_step(CLEAR_STEP_STOP)
	, m_lastScrollMax(false)
{
}

void PlayerStateClear::start(Player* _pPlayer)
{
	_pPlayer->m_animationController.setAnimation(ANIMATION_PLAYER_RUN);
	_pPlayer->m_speed = { 0.0f, 0.0f };
	_pPlayer->m_messageController.setMessage(messages, messageSize);

	g_courseManager.destroyBridge();
}

void PlayerStateClear::update(PlayerStateContext* _pStateContext, Player* _pPlayer)
{
	switch (m_step) {
	case CLEAR_STEP_STOP:
		if (g_enemyManager.koopaIsDead()) {
			m_step = CLEAR_STEP_MOVE;
			g_pSound->play(SOUND_WORLED_CLEAR);
		}
		break;
	case CLEAR_STEP_MOVE:
		_pPlayer->m_acceleration = 0.14f;
		_pPlayer->m_speed.x += _pPlayer->m_acceleration;
		_pPlayer->m_speed.y += PLAYER_GRAVITY;
		{
			static int one = 0x100;
			ivec2 fixed = _pPlayer->m_speed * (float)one;
			fixed = fixed * 0xe8 / one;
			_pPlayer->m_speed = (vec2)fixed / (float)one;
		}

		// ƒLƒmƒsƒI‚Æ‚Ì“–‚½‚è”»’è
		if (g_courseManager.intersectKinopio(_pPlayer)) {
			_pPlayer->m_speed = { 0.0f, 0.0f };
			_pPlayer->m_animationController.setAnimation(ANIMATION_PLAYER_IDLE);
			m_step = CLEAR_STEP_STAY;
		}
		break;
	case CLEAR_STEP_STAY:
		g_courseManager.addScroll(1.0f);
		if (g_courseManager.isScrollMax() && (!m_lastScrollMax)) {
			_pPlayer->m_messageController.start();
		}
		m_lastScrollMax = g_courseManager.isScrollMax();
		break;

	default:
		break;
	}
}
