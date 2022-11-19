#include "PlayerStateIdle.h"
#include "PlayerStateRun.h"
#include "PlayerStateJump.h"
#include "../Keyboard.h"
#include "../animation/Animation.h"

PlayerStateIdle::PlayerStateIdle()
	: PlayerState(PLAYER_STATE_IDLE)
{
}

void PlayerStateIdle::update(PlayerStateContext* _pStateContext, Player* _pPlayer)
{
	if (Keyboard::m_pressed[PLAYER_KEY_RIGHT] || Keyboard::m_pressed[PLAYER_KEY_LEFT]) {
		_pPlayer->m_animeCtr.setAnimation(ANIMATION_PLAYER_RUN);
		_pStateContext->setStete(new PlayerStateRun);
	}

	if (Keyboard::m_nowPressed[PLAYER_KEY_JUMP]) {
		_pPlayer->m_animeCtr.setAnimation(ANIMATION_PLAYER_JUMP);
		_pStateContext->setStete(new PlayerStateJump);
	}
}
