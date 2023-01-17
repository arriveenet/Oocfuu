#pragma once
#include "Rect.h"
#include <glm/glm.hpp>

#define FIREBALL_SIZE			8
#define FIREBAR_WIDTH			6
#define FIREBAR_ROTATE_SPEED	2

enum FIREBAR_ROTATE {
	FIREBAR_CLOCKWISE,
	FIREBAR_ANTICLOCKWISE,
};

class Firebar{
	glm::vec2 m_position;
	float m_angle;
	FIREBAR_ROTATE m_rotate;
	Rect m_fireballs[FIREBAR_WIDTH];

public:
	Firebar(float _x, float _y, FIREBAR_ROTATE _rotate);

	void upadte();
	void draw();
};

