#pragma once

#include "glm/glm.hpp"

using namespace glm;

#define RECT_FLIP_NONE			0
#define RECT_FLIP_HORIZONTAL	(1<<0)
#define RECT_FLIP_VERTICAL		(1<<1)

struct Rect {
	vec2 m_size;
	vec2 m_position;
	int m_flip;

	Rect();
	Rect(float _width, float _height);
	Rect(vec2 const& _size);
	Rect(vec2 const& _size, vec2 const& _position);
	void draw();
	bool intersect(vec2 const& _point);
	bool intersect(Rect const& _rect);
};