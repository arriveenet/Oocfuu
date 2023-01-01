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

class Firebar : public Rect{
public:
	float m_angle;
	FIREBAR_ROTATE m_rotate;
	
	Firebar(float _x, float _y, FIREBAR_ROTATE _rotate);

	void upadte();
	void draw();
};

