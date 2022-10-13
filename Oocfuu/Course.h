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

struct Course {
private:
	int m_width;
	int m_height;
	std::vector<QUAD> m_quads;
	GLuint m_texture;

public:
	int **m_parts;
	float m_scroll;

	Course(int _width, int _height);
	~Course();
	int load(const char* _fileName);
	int reload(const char* _fileName, int _width, int _height);
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