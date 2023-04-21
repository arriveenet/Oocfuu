#pragma once
#include "PlayerState.h"
#include "Player.h"
#include <string>

class PlayerState;
class Player;

enum PLAYER_STATE;

class PlayerStateContext
{
	PlayerState* m_pState;
	PLAYER_STATE m_currentState;
	Player* m_pPlayer;

public:
	explicit PlayerStateContext(Player* _pPlayer);
	virtual ~PlayerStateContext();

	void setStete(PlayerState* _pState);
	void update();
	std::string getString();
	PLAYER_STATE getStateEnum();
};

