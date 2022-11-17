#pragma once
#include "../Sprite.h"
#include "PlayerStateContext.h"
#include "PlayerState.h"

#include <glm/glm.hpp>

#define PLAYER_KEY_DOWN		's'
#define PLAYER_KEY_LEFT		'a'
#define PLAYER_KEY_RIGHT	'd'
#define PLAYER_KEY_JUMP		' '

#define PLAYER_JUMP_COUNT_MAX	15

#define PLAYER_DEFAULT_X		40
#define PLAYER_DEFAULT_Y		192

class Player;
class PlayerStateContext;

class Player : public Sprite{
	PlayerStateContext* m_pStateContext;

public:
	Player();
	~Player();

	void init();
	void reset();
	void update();
	void draw();
};

extern Player g_player;