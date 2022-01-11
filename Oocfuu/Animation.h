#pragma once

#include <vector>

enum {
	ANIMATION_NONE = -1,
	ANIMATION_PLAYER_IDLE,
	ANIMATION_PLAYER_RUN,
	ANIMATION_PLAYER_BRAKE,
	ANIMATION_PLAYER_JUMP,
	ANIMATION_MAX
};

struct Animation {
	std::vector<int> m_keys;
	int m_rate;
	int m_next;

	Animation();
	static int initAll();
};

extern Animation g_animations[ANIMATION_MAX];