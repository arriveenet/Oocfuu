#pragma once

#include "Rect.h"  

enum {
	TEXTURE_PLAYER_IDLE,
	TEXTURE_PLAYER_JUMP,
	TEXTURE_PLAYER_RUN_1,
	TEXTURE_PLAYER_RUN_2,
	TEXTURE_CAKE,
	TEXTURE_COIN_1,
	TEXTURE_OOCFUU_HOI,
	TEXTURE_OOCFUU_GG,
	TEXTURE_OOCFUU_HI,
	TEXTURE_FIREWORK_1,
	TEXTURE_FIREWORK_2,
	TEXTURE_FIREWORK_3,
	TEXTURE_MAX
};

struct Texture {
	const char* m_fileName;
	unsigned int m_texture;
	Rect m_bound;

	int init();
	static int initAll();
};

extern Texture g_textures[TEXTURE_MAX];