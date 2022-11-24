#pragma once

#include <vector>
#include <gl/glut.h>
#include <glm/glm.hpp>

#define COURSE_WIDTH	211
#define COURSE_HEIGHT	15

typedef struct {
	glm::vec2 position;
	glm::vec2 texCoord;
}VERTEX;

typedef struct {
	VERTEX vertices[4];
}QUAD;

class Course {
	int m_width;
	int m_height;
	int** m_pParts;
	std::vector<QUAD> m_quads;

public:
	float m_scroll;

	Course();
	~Course();

	bool init(int _width, int _height);
	void release();
	bool load(const char* _fileName);
	bool reload(const char* _fileName, int _width, int _height);
	void update();
	void draw();
	void setParts(glm::ivec2 const& _point, int _parts);
	int getParts(int _x, int _y);
	bool intersect(glm::vec2 const& _point);
	bool intersect(glm::vec2 const& _point, int* _parts);
	int getWidth();
	int getHeight();
};

extern Course g_course;