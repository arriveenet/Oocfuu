#include "Lift.h"
#include "font.h"

#include <glm/glm.hpp>

using namespace glm;

Lift::Lift(float _x, float _y, int _width)
	: m_width(_width)
	, Rect(vec2(LIFT_SIZE * _width, LIFT_SIZE), vec2(_x, _y))
{
}

void Lift::update()
{
	m_position.x += 1;
}

void Lift::draw()
{
	fontBegin();
	for (int i = 0; i < m_width; i++) {
		fontPosition(m_position.x + (i * LIFT_SIZE), m_position.y);
		fontDraw("%c", 'j');
	}
	fontEnd();
}
