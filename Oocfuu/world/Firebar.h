#pragma once
#include <glm/glm.hpp>

class Firebar {
public:
	glm::vec2 m_position;
	float m_angle;
	
	Firebar(float _x, float _y);

	void upadte();
	void draw();
};

