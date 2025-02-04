#include "Firework.h"
#include "animation/Animation.h"
#include "TextureManager.h"
#include "sound/Sound.h"

#include <GL/freeglut.h>
#include <al.h>

#define FIREWORK_COUNT_MAX	60

using namespace glm;

Firework g_firework;

Firework::Firework()
	: m_ac()
	, m_textures{0}
	, m_texture(0)
	, m_countLeft(0)
	, m_explosion(false)
{}

int Firework::init()
{
	m_position = {0, 0};
	m_size = { 16,16 };
	m_explosion = false;
	m_ac.setAnimation(ANIMATION_FIREWORK);

	//for (int i = 0; i < 3; i++)
		//m_textures[i] = g_textures[TEXTURE_FIREWORK_1 + i].m_texture;

	return 0;
}

void Firework::start(vec2 const& _position)
{
	m_position = _position;
	m_explosion = true;
	m_countLeft = FIREWORK_COUNT_MAX;
	m_ac.setAnimation(ANIMATION_FIREWORK);
	//alSourcePlay(g_soundManager[SOUND_FIREWORK].m_sid);
}

void Firework::update()
{
	if (m_countLeft > 0) {
		m_countLeft--;
		m_ac.update();
	}
	else
	{
		m_explosion = false;
	}
}

void Firework::draw()
{
	if (m_countLeft <= 0)
		return;

	//Texture texture = g_textures[g_animations[m_ac.m_animation].m_keys[m_ac.m_time]];
	//glBindTexture(
	//	GL_TEXTURE_2D,
	//	texture.m_texture);

	Rect::draw();
}