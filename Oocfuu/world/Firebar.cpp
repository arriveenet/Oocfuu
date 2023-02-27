#include "FireBar.h"

#include "App.h"
#include "common/font.h"
#include "Game.h"
#include "TextureManager.h"
#include "world/Part.h"
#include "world/Course.h"
#include "player/Player.h"

#include <cmath>
#include <gl/GL.h>
#include <gl/glut.h>

using namespace glm;

Firebar::Firebar(float _x, float _y, FIREBAR_ROTATE _rotate)
	: m_angle(0.0f)
	, m_rotate(_rotate)
	, m_fireballAngle(0.0f)
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

	if (Game::m_count % 8 == 0) {
		m_fireballAngle += 90.0f;
		//printf("m_fireballAngle=%f\n", m_fireballAngle);
		if (m_fireballAngle >= 360) {
			m_fireballAngle = 0.0f;
		}
	}
}

void Firebar::draw()
{
	if ((g_courseManager.m_scroll > m_position.x + (PART_SIZE * 4))
		|| (g_courseManager.m_scroll + SCREEN_WIDTH + (PART_SIZE * 4) < m_position.x)
		)
		return;

	GLint lastMatrixMode;
	glGetIntegerv(
		GL_MATRIX_MODE,		// GLenum pname
		&lastMatrixMode);	// GLint *params

	glMatrixMode(GL_TEXTURE);// GLenum mode
	glLoadIdentity();
	glPushMatrix();
	glRotatef(
		m_fireballAngle,	// GLfloat angle
		0, 0, 1				// GLfloat x, y, z
	);

	g_textureManager.setTexture(TEXTURE_FIREBALL);
	for (int i = 0; i < FIREBAR_WIDTH; i++) {
		vec2 position = 
			{ m_position.x + cosf(radians(m_angle)) * (FIREBALL_SIZE * i),
			m_position.y + sinf(radians(m_angle)) * (FIREBALL_SIZE * i) };

		m_fireballs[i] = Rect(vec2(FIREBALL_SIZE, FIREBALL_SIZE), position);
		m_fireballs[i].draw();

		if (m_fireballs[i].intersect(g_player))
			g_player.kill();

		if (Game::m_debugInfo)
			m_fireballs[i].drawWire();
	}

	glPopMatrix();
	glMatrixMode(lastMatrixMode);
	g_textureManager.unbindTexture();
}
