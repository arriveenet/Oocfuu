#include "PlayerStateContext.h"
#include "PlayerStateRun.h"
#include <stdio.h>

PlayerStateContext::PlayerStateContext()
	: m_pState(NULL)
{
}

PlayerStateContext::~PlayerStateContext()
{
	if (m_pState) {
		delete m_pState;
		m_pState = NULL;
	}
}

void PlayerStateContext::setStete(PlayerState* _pState)
{
	if (m_pState) {
		delete m_pState;
		m_pState = NULL;
	}

	m_pState = _pState;
}

void PlayerStateContext::update(Player* _pPlayer)
{
	m_pState->update(this, _pPlayer);
}
