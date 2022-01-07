#pragma once

#include "Rect.h"

enum {
	TEXTURE_SMALLMARIO_BRAKE,
	TEXTURE_SMALLMARIO_DIE,
	TEXTURE_SMALLMARIO_FLAGPOLE_1,
	TEXTURE_SMALLMARIO_FLAGPOLE_2,
	TEXTURE_SMALLMARIO_IDLE,
	TEXTURE_SMALLMARIO_JUMP,
	TEXTURE_SMALLMARIO_RUN_1,
	TEXTURE_SMALLMARIO_RUN_2,
	TEXTURE_SMALLMARIO_RUN_3,
	TEXTURE_CAKE,
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