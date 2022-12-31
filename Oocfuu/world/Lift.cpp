#include "Lift.h"

#include "App.h"
#include "Part.h"
#include "Course.h"
#include "TextureManager.h"

#include <glm/glm.hpp>

using namespace glm;

Lift::Lift(float _x, float _y, int _width, LIFTMODE _mode)
	: m_width(_width)
	, m_mode(_mode)
	, m_speed(0, 0)
	, m_startPosition(_x, _y)
	, Rect(vec2(LIFT_SIZE * _width, LIFT_SIZE), vec2(_x, _y))
{
	switch (_mode) {
	case LIFTMODE_LEFTANDRIGHT:
		m_speed = {LIFT_SPEED, 0};
		break;
	case LIFTMODE_UPANDDOWN:
		m_speed = { 0, LIFT_SPEED };
		break;
	}
}

void Lift::update()
{
	if ((g_courseManager.m_scroll > m_position.x + m_size.x + (PART_SIZE * 4))
		|| (g_courseManager.m_scroll + SCREEN_WIDTH + (PART_SIZE * 4) < m_position.x)
		)
		return;

	if (m_position.x > m_startPosition.x + LIFT_DISTANCE
		|| m_position.x < m_startPosition.x
		|| m_position.y > m_startPosition.y + LIFT_DISTANCE
		|| m_position.y < m_startPosition.y) {
		switch (m_mode) {
		case LIFTMODE_LEFTANDRIGHT:
			m_speed.x = m_speed.x > 0 ? -LIFT_SPEED : LIFT_SPEED;
			break;
		case LIFTMODE_UPANDDOWN:
			m_speed.y = m_speed.y > 0 ? -LIFT_SPEED : LIFT_SPEED;
			break;
		}
	}

	m_position += m_speed;
	
}

void Lift::draw()
{
	if ((g_courseManager.m_scroll > m_position.x + m_size.x + (PART_SIZE * 4))
		|| (g_courseManager.m_scroll + SCREEN_WIDTH + (PART_SIZE * 4) < m_position.x)
		)
		return;

	g_textureManager.setTexture(TEXTURE_LIFT);
	for (int i = 0; i < m_width; i++) {
		Rect(vec2(LIFT_SIZE, LIFT_SIZE), vec2(m_position.x + (i * LIFT_SIZE), m_position.y)).draw();
	}
	g_textureManager.unbindTexture();
}

bool Lift::intersect(glm::vec2 const& _point)
{
	return Rect::intersect(_point);
}
