#include "PlayerStateDie.h"
#include "Game.h"
#include "Course.h"
#include "App.h"
#include "sound/Sound.h"

PlayerStateDie::PlayerStateDie()
	: PlayerState(PLAYER_STATE_DIE)
	, m_counter(0)
{
}

void PlayerStateDie::start(Player* _pPlayer)
{
	g_pSound->play(SOUND_SE_DIE);
	_pPlayer->m_speed = { 0, 0 };
	m_counter = 0;
}

void PlayerStateDie::update(PlayerStateContext* _pStateContext, Player* _pPlayer)
{
	m_counter++;

	// 落下死でなければ死亡アニメーションをする
	if (_pPlayer->m_position.y <= SCREEN_HEIGHT) {
		if (m_counter > 20) {
			_pPlayer->m_speed.y = +2.5f;
		} else {
			_pPlayer->m_speed.y = -2.8f;
		}
	}

	if (m_counter >= 60 * 2) {
		_pPlayer->m_left--;
		if (_pPlayer->m_left <= 0)
			g_game.m_isGameOver = true;
		g_game.setScreen(GAME_SCREEN_INTRO);
	}
}
