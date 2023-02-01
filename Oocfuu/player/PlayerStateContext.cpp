#include "PlayerStateContext.h"
#include "PlayerStateRun.h"
#include <stdio.h>

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

PlayerStateContext::PlayerStateContext(Player* _pPlayer)
	: m_pState(NULL)
	, m_currentState(PLAYER_STATE_IDLE)
	, m_pPlayer(_pPlayer)
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
	m_currentState = _pState->getState();
	m_pState->start(m_pPlayer);
	//printf("state=%s\n", getString().c_str());
}

void PlayerStateContext::update()
{
	m_pState->update(this, m_pPlayer);
}

std::string PlayerStateContext::getString()
{
	std::string str;

	switch (m_currentState) {
	case PLAYER_STATE_IDLE:
		str = "PLAYER_STATE_IDLE";
		break;
	case PLAYER_STATE_RUN:
		str = "PLAYER_STATE_RUN";
		break;
	case PLAYER_STATE_JUMP:
		str = "PLAYER_STATE_JUMP";
		break;
	case PLAYER_STATE_DIE:
		str = "PLAYER_STATE_DIE";
		break;
	case PLAYER_STATE_GOAL:
		str = "PLAYER_STATE_GOAL";
		break;
	case PLAYER_STATE_CLEAR:
		str = "PLAYER_STATE_CLEAR";
	}

	return str;
}

PLAYER_STATE PlayerStateContext::getStateEnum()
{
	return m_currentState;
}
