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
	explicit Rect(glm::vec2 const& _size);
	Rect(float _width, float _height);
	Rect(glm::vec2 const& _size, glm::vec2 const& _position);

	virtual void draw();
	virtual void drawWire();
	virtual bool intersect(glm::vec2 const& _point);
	virtual bool intersect(Rect const& _rect);

	bool operator==(const Rect& _rect);
	bool operator!=(const Rect& _rect);
};