#include "Lift.h"

#include "App.h"
#include "world/Part.h"
#include "world/Course.h"
#include "TextureManager.h"

#include <glm/glm.hpp>

using namespace glm;

Lift::Lift(float _x, float _y, int _width, LIFT_MOVEMENT _mode, int _distance)
	: m_width(_width)
	, m_distance(_distance)
	, m_mode(_mode)
	, m_speed(0, 0)
	, m_startPosition(_x, _y)
	, Rect(vec2(LIFT_SIZE * _width, LIFT_SIZE), vec2(_x, _y))
{
	switch (_mode) {
	case LIFT_LEFT_AND_RIGHT:
		m_speed = {LIFT_SPEED, 0};
		break;
	case LIFT_UP_AND_DOWN:
		m_speed = { 0, LIFT_SPEED };
		break;
	case LEFT_SCREEN_DOWN:
		m_speed = { 0, LIFT_SPEED };
		break;
	case LEFT_SCREEN_UP:
		m_speed = { 0, -LIFT_SPEED };
		break;
	}
}

void Lift::update()
{
	if ((g_courseManager.getScroll() > m_position.x + m_size.x + (PART_SIZE * 4))
		|| (g_courseManager.getScroll() + SCREEN_WIDTH + (PART_SIZE * 4) < m_position.x)
		)
		return;

	if (m_mode == LEFT_SCREEN_UP) {
		if (m_position.y + m_size.y < 0) {
			m_position.y = SCREEN_HEIGHT;
		}
	}
	else if (m_mode == LEFT_SCREEN_DOWN) {
		if (m_position.y > SCREEN_HEIGHT) {
			m_position.y =  -m_size.y;
		}
	} else {
		if (m_position.x > m_startPosition.x + m_distance
			|| m_position.x < m_startPosition.x
			|| m_position.y > m_startPosition.y + m_distance
			|| m_position.y < m_startPosition.y) {
			switch (m_mode) {
			case LIFT_LEFT_AND_RIGHT:
				m_speed.x = m_speed.x > 0 ? -LIFT_SPEED : LIFT_SPEED;
				break;
			case LIFT_UP_AND_DOWN:
				m_speed.y = m_speed.y > 0 ? -LIFT_SPEED : LIFT_SPEED;
				break;
			}
		}
	}

	m_position += m_speed;
	
}

void Lift::draw()
{
	if ((g_courseManager.getScroll() > m_position.x + m_size.x + (PART_SIZE * 4))
		|| (g_courseManager.getScroll() + SCREEN_WIDTH + (PART_SIZE * 4) < m_position.x)
		)
		return;

	g_textureManager.setTexture(TEXTURE_LIFT);
	for (int i = 0; i < m_width; i++) {
		Rect(vec2(LIFT_SIZE, LIFT_SIZE), vec2(m_position.x + (i * LIFT_SIZE), m_position.y)).draw();
	}
	g_textureManager.unbindTexture();

	if (Game::m_debugInfo) {
		Rect::drawWire();
	}
}

bool Lift::intersect(glm::vec2 const& _point)
{
	return Rect::intersect(_point);
}
