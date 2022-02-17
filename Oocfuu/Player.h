#pragma once

#include <vector>
#include "Rect.h"
#include "AnimationController.h"

#define PLAYER_SIZE	16

#define PLAYER_KEY_DOWN		's'
#define PLAYER_KEY_LEFT		'a'
#define PLAYER_KEY_RIGHT	'd'
#define PLAYER_KEY_JUMP		' '

#define PLAYER_JUMP_COUNT_MAX	15

#define PLAYER_DEFAULT_X		40
#define PLAYER_DEFAULT_Y		192

struct Player :Rect {
	AnimationController m_animationController;
	vec2 m_speed;
	bool m_jumping;
	bool m_falling;
	int m_jumpCount;
	int m_left;
	unsigned int m_counter;
	std::vector<vec2>m_leftPoints;
	std::vector<vec2>m_rightPoints;
	std::vector<vec2>m_bottomPoints;
	std::vector<vec2>m_topPoints;

	Player();
	int init();
	void reset();
	void update();
	void draw();
	void draw(vec2 _position);
};

extern Player g_player;