#pragma once
#include <gl/glut.h>
#include <glm/glm.hpp>

enum TEXTURE
{
	TEXTURE_PLAYER_IDLE,
	TEXTURE_PLAYER_JUMP,
	TEXTURE_PLAYER_DIE,
	TEXTURE_PLAYER_RUN_1,
	TEXTURE_PLAYER_RUN_2,
	TEXTURE_PLAYER_RUN_3,
	TEXTURE_PLAYER_FLAG_POLE_1,
	TEXTURE_PLAYER_FLAG_POLE_2,
	TEXTURE_KURIBO_RUN,
	TEXTURE_KURIBO_SQUISH,
	TEXTURE_CAKE,
	TEXTURE_KINOPIO,
	TEXTURE_COIN_1,
	TEXTURE_FIREWORK_1,
	TEXTURE_FIREWORK_2,
	TEXTURE_FIREWORK_3,
	TEXTURE_GOAL_FLAG,
	TEXTURE_TITLE,
	TEXTURE_BLOCK_COIN_1,
	TEXTURE_BLOCK_COIN_2,
	TEXTURE_BLOCK_COIN_3,
	TEXTURE_BLOCK_COIN_4,
	TEXTURE_PARTS_OVERWORLD,
	TEXTURE_PARTS_UNDERGROUND,
	TEXTURE_PARTS_CASTLE,
	TEXTURE_FIREBALL,
	TEXTURE_LIFT,
	TEXTURE_FONT,
	TEXTURE_FONT_BMP,
	TEXTURE_SCORE_100,
	TEXTURE_SCORE_200,
	TEXTURE_SCORE_400,
	TEXTURE_SCORE_500,
	TEXTURE_SCORE_800,
	TEXTURE_SCORE_1000,
	TEXTURE_SCORE_2000,
	TEXTURE_SCORE_4000,
	TEXTURE_SCORE_5000,
	TEXTURE_SCORE_8000,
	TEXTURE_KOOPA_IDLE_1,
	TEXTURE_KOOPA_IDLE_2,
	TEXTURE_KOOPA_IDLE_FIRE_1,
	TEXTURE_KOOPA_IDLE_FIRE_2,
	TEXTURE_KOOPA_FIRE_1,
	TEXTURE_KOOPA_FIRE_2,
	TEXTURE_NOKONOKO_RETURN,
	TEXTURE_NOKONOKO_RUN_1,
	TEXTURE_NOKONOKO_RUN_2,
	TEXTURE_NOKONOKO_SQUISH,
	TEXTURE_RED_NOKONOKO_RETURN,
	TEXTURE_RED_NOKONOKO_RUN_1,
	TEXTURE_RED_NOKONOKO_RUN_2,
	TEXTURE_RED_NOKONOKO_SQUISH,
	TEXTURE_MAX
};

typedef struct tagTEX {
	char fileName[64];
	COLORREF colorKey;
	glm::ivec2 size;
} TEX;

class TextureManager
{
	GLuint m_textures[TEXTURE_MAX];

public:
	TextureManager();
	~TextureManager();

	bool init();
	void release();
	void setTexture(enum TEXTURE _texture);
	void setTexture(GLuint _texture);
	GLuint getTexture(enum TEXTURE _texture);
	glm::ivec2 getSize(enum TEXTURE _texture);

	void unbindTexture();
};

extern TextureManager g_textureManager;

