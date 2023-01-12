#include "PlayerStateDie.h"
#include "Game.h"
#include "Course.h"

PlayerStateDie::PlayerStateDie()
	: PlayerState(PLAYER_STATE_DIE)
	, m_counter(0)
{
}

void PlayerStateDie::start(Player* _pPlayer)
{
}

void PlayerStateDie::update(PlayerStateContext* _pStateContext, Player* _pPlayer)
{
	m_counter++;

	if (m_counter >= 60 * 2) {
		_pPlayer->m_left--;
		_pPlayer->m_dead = false;
		if (_pPlayer->m_left <= 0)
			g_game.m_isGameOver = true;
		g_courseManager.m_scroll = 0.0f;
		g_game.setScreen(GAME_SCREEN_INTRO);
	}
}
