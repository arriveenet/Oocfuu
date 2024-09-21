#pragma once
#include <glm/glm.hpp>

class Renderer {
public:
	Renderer();
	~Renderer();

	void draw();

private:
	glm::vec4 m_clearColor;
};

