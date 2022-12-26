#include "FireBar.h"
#include "font.h"
#include "Game.h"

#include <cmath>

using namespace glm;

Firebar::Firebar(float _x, float _y)
	: m_position(_x, _y)
	, m_angle(0.f)
{
}

void Firebar::upadte()
{
	m_angle += 2;
}

void Firebar::draw()
{
	unsigned char fire = 'f';
	const int len = 6;
	unsigned char table[len] = { 0,1,2,2,1,0 };
	fire += table[(g_game.m_count / 4) % len];

	fontBegin();
	for (int i = 0; i < 6; i++) {
		fontPosition(m_position.x + cosf(radians(m_angle)) * (8 * i), m_position.y + sinf(radians(m_angle)) * (8 * i));
		fontDraw("%c", fire);
	}
	fontEnd();
}
