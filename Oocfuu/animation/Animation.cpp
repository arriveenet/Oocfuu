#include "Animation.h"
#include "../Texture.h"

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

	for (int i = TEXTURE_FIREWORK_1; i <= TEXTURE_FIREWORK_3; i++) {
		g_animations[ANIMATION_FIREWORK].m_keys.push_back(i);
		g_animations[ANIMATION_FIREWORK].m_rate = 4;
		g_animations[ANIMATION_FIREWORK].m_next = ANIMATION_NONE;
	}

	return 0;
}