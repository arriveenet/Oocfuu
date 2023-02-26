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
	for (int i = TEXTURE_PLAYER_RUN_1; i <= TEXTURE_PLAYER_RUN_2; i++) {
		g_animations[ANIMATION_PLAYER_RUN].m_keys.push_back(i);
		g_animations[ANIMATION_PLAYER_RUN].m_rate = 8;
		g_animations[ANIMATION_PLAYER_RUN].m_next = ANIMATION_PLAYER_RUN;
	}

	/* Jump animation */
	g_animations[ANIMATION_PLAYER_JUMP].m_keys.push_back(TEXTURE_PLAYER_JUMP);
	g_animations[ANIMATION_PLAYER_JUMP].m_rate = 15;
	g_animations[ANIMATION_PLAYER_JUMP].m_next = ANIMATION_PLAYER_JUMP;

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

	/* Firework animation */
	for (int i = TEXTURE_FIREWORK_1; i <= TEXTURE_FIREWORK_3; i++) {
		g_animations[ANIMATION_FIREWORK].m_keys.push_back(i);
		g_animations[ANIMATION_FIREWORK].m_rate = 4;
		g_animations[ANIMATION_FIREWORK].m_next = ANIMATION_NONE;
	}


	return 0;
}

