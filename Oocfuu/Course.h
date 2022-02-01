#pragma once

#include "glm/glm.hpp"

#define COURSE_WIDTH	16
#define COURSE_HEIGHT	15

struct Course {
private:
	int m_width;
	int m_height;

public:
	int **m_parts;
	float m_scroll;

	Course(int _width, int _height);
	~Course();
	int load(const char* _fileName);
	void draw();
	void setParts(glm::ivec2 const& _point, int _parts);
	int getParts(int _x, int _y);
	bool intersect(glm::vec2 const& _point);
	bool intersect(glm::vec2 const& _point, int* _parts);
};

extern Course g_course;