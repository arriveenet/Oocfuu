#include "FireBar.h"

#include "App.h"
#include "font.h"
#include "Game.h"
#include "TextureManager.h"
#include "Part.h"
#include "Course.h"

#include <cmath>
#include <gl/glut.h>

using namespace glm;

Firebar::Firebar(float _x, float _y, FIREBAR_ROTATE _rotate)
	: m_angle(0.0f)
	, m_rotate(_rotate)
	, m_position(_x, _y)
{
}

void Firebar::upadte()
{
	if ((g_courseManager.m_scroll > m_position.x + (PART_SIZE * 4))
		|| (g_courseManager.m_scroll + SCREEN_WIDTH + (PART_SIZE * 4) < m_position.x)
		)
		return;

	switch (m_rotate) {
	case FIREBAR_CLOCKWISE:
		m_angle += FIREBAR_ROTATE_SPEED;
		break;
	case FIREBAR_ANTICLOCKWISE:
		m_angle -= FIREBAR_ROTATE_SPEED;
		break;
	default:
		break;
	}
}

void Firebar::draw()
{
	if ((g_courseManager.m_scroll > m_position.x + (PART_SIZE * 4))
		|| (g_courseManager.m_scroll + SCREEN_WIDTH + (PART_SIZE * 4) < m_position.x)
		)
		return;

	g_textureManager.setTexture(TEXTURE_FIREBALL);
	for (int i = 0; i < FIREBAR_WIDTH; i++) {
		vec2 position = 
			{ m_position.x + cosf(radians(m_angle)) * (FIREBALL_SIZE * i),
			m_position.y + sinf(radians(m_angle)) * (FIREBALL_SIZE * i) };

		Rect fireball = Rect(vec2(FIREBALL_SIZE, FIREBALL_SIZE), position);
		fireball.draw();
		if (Game::m_debugInfo)
			fireball.drawWire();
	}
	g_textureManager.unbindTexture();
}
