#include "Animation.h"
#include "../TextureManager.h"

using namespace std;

Animation g_animations[ANIMATION_MAX];

Animation::Animation()
	: m_next(0)
	, m_rate()
{}

int Animation::initAll()
{
	
	/* Idle animation */
	g_animations[ANIMATION_PLAYER_IDLE].m_keys.push_back(TEXTURE_PLAYER_IDLE);
	g_animations[ANIMATION_PLAYER_IDLE].m_rate = 15;
	g_animations[ANIMATION_PLAYER_IDLE].m_next = ANIMATION_PLAYER_IDLE;

	/* Run animation */
	for (int i = TEXTURE_PLAYER_RUN_1; i <= TEXTURE_PLAYER_RUN_3; i++) {
		g_animations[ANIMATION_PLAYER_RUN].m_keys.push_back(i);
		g_animations[ANIMATION_PLAYER_RUN].m_rate = 6;
		g_animations[ANIMATION_PLAYER_RUN].m_next = ANIMATION_PLAYER_RUN;
	}

	/* Jump animation */
	g_animations[ANIMATION_PLAYER_JUMP].m_keys.push_back(TEXTURE_PLAYER_JUMP);
	g_animations[ANIMATION_PLAYER_JUMP].m_rate = 15;
	g_animations[ANIMATION_PLAYER_JUMP].m_next = ANIMATION_PLAYER_JUMP;

	/* Die animation */
	g_animations[ANIMATION_PLAYER_DIE].m_keys.push_back(TEXTURE_PLAYER_DIE);
	g_animations[ANIMATION_PLAYER_DIE].m_rate = 15;
	g_animations[ANIMATION_PLAYER_DIE].m_next = ANIMATION_PLAYER_DIE;

	/* Flag pole animation */
	for (int i = TEXTURE_PLAYER_FLAG_POLE_1; i <= TEXTURE_PLAYER_FLAG_POLE_2; i++) {
		g_animations[ANIMATION_PLAYER_FLAG_POLE].m_keys.push_back(i);
		g_animations[ANIMATION_PLAYER_FLAG_POLE].m_rate = 8;
		g_animations[ANIMATION_PLAYER_FLAG_POLE].m_next = ANIMATION_PLAYER_FLAG_POLE;
	}

	/* Koopa animation */
	for (int i = TEXTURE_KOOPA_IDLE_1; i <= TEXTURE_KOOPA_IDLE_2; i++) {
		g_animations[ANIMATION_KOOPA].m_keys.push_back(i);
		g_animations[ANIMATION_KOOPA].m_rate = 32;
		g_animations[ANIMATION_KOOPA].m_next = ANIMATION_KOOPA;
	}

	/* Koopa fire animation */
	for (int i = TEXTURE_KOOPA_IDLE_FIRE_1; i <= TEXTURE_KOOPA_IDLE_FIRE_2; i++) {
		g_animations[ANIMATION_KOOPA_FIRE].m_keys.push_back(i);
		g_animations[ANIMATION_KOOPA_FIRE].m_rate = 16;
		g_animations[ANIMATION_KOOPA_FIRE].m_next = ANIMATION_KOOPA;
	}

	/* Nokonoko run */
	for (int i = TEXTURE_NOKONOKO_RUN_1; i <= TEXTURE_NOKONOKO_RUN_2; i++) {
		g_animations[ANIMATION_NOKONOKO_RUN].m_keys.push_back(i);
		g_animations[ANIMATION_NOKONOKO_RUN].m_rate = 16;
		g_animations[ANIMATION_NOKONOKO_RUN].m_next = ANIMATION_NOKONOKO_RUN;
	}

	/* Nokonoko shell */
	g_animations[ANIMATION_NOKONOKO_SHELL].m_keys.push_back(TEXTURE_NOKONOKO_SQUISH);
	g_animations[ANIMATION_NOKONOKO_SHELL].m_rate = 16;
	g_animations[ANIMATION_NOKONOKO_SHELL].m_next = ANIMATION_NOKONOKO_SHELL;

	/* Nokonoko return */
	g_animations[ANIMATION_NOKONOKO_RETURN].m_keys.push_back(TEXTURE_NOKONOKO_SQUISH);
	g_animations[ANIMATION_NOKONOKO_RETURN].m_keys.push_back(TEXTURE_NOKONOKO_RETURN);
	g_animations[ANIMATION_NOKONOKO_RETURN].m_rate = 8;
	g_animations[ANIMATION_NOKONOKO_RETURN].m_next = ANIMATION_NOKONOKO_RETURN;

	/* Red Nokonoko run */
	for (int i = TEXTURE_RED_NOKONOKO_RUN_1; i <= TEXTURE_RED_NOKONOKO_RUN_2; i++) {
		g_animations[ANIMATION_RED_NOKONOKO_RUN].m_keys.push_back(i);
		g_animations[ANIMATION_RED_NOKONOKO_RUN].m_rate = 16;
		g_animations[ANIMATION_RED_NOKONOKO_RUN].m_next = ANIMATION_RED_NOKONOKO_RUN;
	}

	/* Red Nokonoko shell */
	g_animations[ANIMATION_RED_NOKONOKO_SHELL].m_keys.push_back(TEXTURE_RED_NOKONOKO_SQUISH);
	g_animations[ANIMATION_RED_NOKONOKO_SHELL].m_rate = 16;
	g_animations[ANIMATION_RED_NOKONOKO_SHELL].m_next = ANIMATION_RED_NOKONOKO_SHELL;

	/* Red Nokonoko return */
	g_animations[ANIMATION_RED_NOKONOKO_RETURN].m_keys.push_back(TEXTURE_RED_NOKONOKO_SQUISH);
	g_animations[ANIMATION_RED_NOKONOKO_RETURN].m_keys.push_back(TEXTURE_RED_NOKONOKO_RETURN);
	g_animations[ANIMATION_RED_NOKONOKO_RETURN].m_rate = 8;
	g_animations[ANIMATION_RED_NOKONOKO_RETURN].m_next = ANIMATION_RED_NOKONOKO_RETURN;


	/* Block coin animation */
	for (int i = TEXTURE_BLOCK_COIN_1; i <= TEXTURE_BLOCK_COIN_4; i++) {
		g_animations[ANIMATION_BLOCK_COIN].m_keys.push_back(i);
		g_animations[ANIMATION_BLOCK_COIN].m_rate = 2;
		g_animations[ANIMATION_BLOCK_COIN].m_next = ANIMATION_BLOCK_COIN;
	}

	/* Firework animation */
	for (int i = TEXTURE_FIREWORK_1; i <= TEXTURE_FIREWORK_3; i++) {
		g_animations[ANIMATION_FIREWORK].m_keys.push_back(i);
		g_animations[ANIMATION_FIREWORK].m_rate = 4;
		g_animations[ANIMATION_FIREWORK].m_next = ANIMATION_NONE;
	}


	return 0;
}

