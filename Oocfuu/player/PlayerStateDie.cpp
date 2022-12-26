#include "PlayerStateDie.h"
#include "../Game.h"

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
		if (_pPlayer->m_left <= 0)
			g_game.m_isGameOver = true;
		g_game.setScreen(GAME_SCREEN_INTRO);
	}
}
