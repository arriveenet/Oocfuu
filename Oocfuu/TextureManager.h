#pragma once
#include <gl/glut.h>

enum TEXTURE
{
	TEXTURE_PLAYER_IDLE,
	TEXTURE_PLAYER_JUMP,
	TEXTURE_PLAYER_RUN_1,
	TEXTURE_PLAYER_RUN_2,
	TEXTURE_KURIBO_RUN,
	TEXTURE_KURIBO_SQUISH,
	TEXTURE_CAKE,
	TEXTURE_COIN_1,
	TEXTURE_FIREWORK_1,
	TEXTURE_FIREWORK_2,
	TEXTURE_FIREWORK_3,
	TEXTURE_TITLE,
	TEXTURE_PARTS,
	TEXTURE_FONT,
	TEXTURE_MAX
};

typedef struct tagRGB {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} RGB;

typedef struct tagTEX {
	char fileName[64];
	RGB colorKey;
} TEX;

class TextureManager
{
	GLuint m_textures[TEXTURE_MAX];

public:
	TextureManager();
	~TextureManager();

	bool init();
	void release();
	void setTexture(enum TEXTURE _tex);
	GLuint getTexture(enum TEXTURE _tex);

	void unbindTexture();
};

extern TextureManager g_textureManager;

