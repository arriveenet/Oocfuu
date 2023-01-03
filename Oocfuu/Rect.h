#pragma once

#include <glm/glm.hpp>

#define RECT_FLIP_NONE			0
#define RECT_FLIP_HORIZONTAL	(1<<0)
#define RECT_FLIP_VERTICAL		(1<<1)

struct Rect {
	glm::vec2 m_size;
	glm::vec2 m_position;
	int m_flip;

	Rect();
	Rect(float _width, float _height);
	Rect(glm::vec2 const& _size);
	Rect(glm::vec2 const& _size, glm::vec2 const& _position);
	void draw();
	void drawWire();
	bool intersect(glm::vec2 const& _point);
	bool intersect(Rect const& _rect);
};