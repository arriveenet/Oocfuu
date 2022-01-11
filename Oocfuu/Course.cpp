#include <stdio.h>
#include <string.h>

#include "Course.h"
#include "Part.h"
#include "Rect.h"
#include "Header.h"

using namespace glm;

Course g_course{COURSE_WIDTH, COURSE_HEIGHT};

Course::Course(int _width, int _height)
	: m_width(_width)
	, m_height(_height)
	, m_scroll(0.f)
{
	m_parts = new int*[m_height];
	for (int i = 0; i < m_height; i++) {
		m_parts[i] = new int[m_width];
	}
	
}

Course::~Course()
{
	for (int i = 0; i < m_height; ++i) {
		delete m_parts[i];
	}
	delete[] m_parts;
}

int Course::load(const char* _fileName) {
	FILE* pFile;
	errno_t error;

	error = fopen_s(&pFile, _fileName, "r");
	if (error != 0) {
		printf("%s open failed!\n", _fileName);
		return 1;
	}
	printf("%s opened!\n", _fileName);

	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			char buf[2];
			fread(buf, sizeof(char), 2, pFile);
			//printf("%d-%d %c%c", i, j, buf[0], buf[1]);
			if (buf[0] == 0x20) {
				m_parts[i][j] = PART_NONE;
			}
			else
				for (int k = PART_NONE+1; k < PART_MAX; k++) {
					if (strncmp(buf, g_parts[k].m_fileName, 2) == 0) {
						m_parts[i][j] = k;
						break;
					}
				}
		}
		fseek(pFile, 2, SEEK_CUR);
	}
	fclose(pFile);

	return 0;
}

void Course::draw() {
	int scrolleColumn = (int)m_scroll / PART_SIZE;
	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++) {
			if (
				(j < scrolleColumn)
				|| (j > scrolleColumn + (SCREEN_WIDTH / PART_SIZE))
				)
				continue;

			int part = m_parts[i][j];
			switch (part){
			case PART_NONE:
				break;
			default:
			{
				int textureIndex = part;
				GLuint texture = g_parts[textureIndex].m_texture;

				glBindTexture(
					GL_TEXTURE_2D,	// GLenum target
					texture);		// GLuint 
				Rect(vec2(PART_SIZE, PART_SIZE), vec2(j, i) * (float)PART_SIZE).draw();
			}
				break;
			}
		}
}

void Course::setParts(ivec2 const& _point, int _parts) {
	if (
		(_point.x < 0)
		|| (_point.x >= m_width)
		|| (_point.y < 0)
		|| (_point.y >= m_height)
		)
		return;

	m_parts[_point.y][_point.x] = _parts;
}

int Course::getParts(int _x, int _y) {
	if (_x < 0
		|| _x >= m_width
		|| _y < 0
		|| _y >= m_height)
		return PART_NONE;

	return m_parts[_y][_x];
}

bool Course::intersect(vec2 const& _point) {
	ivec2 cellPoint = (ivec2)_point / PART_SIZE;
	if (
		(cellPoint.x < 0)
		|| (cellPoint.x >= m_width)
		|| (cellPoint.y < 0)
		|| (cellPoint.y >= m_height)
		)
		return false;

	switch (m_parts[cellPoint.y][cellPoint.x]) {
	//case PART_NONE:
	case PART_GROUND:
	case PART_HARD_BLOCK:
	case PART_HARD_BLOCK_2:
	case PART_SOFT_BLOCK:
	case PART_PIPE_UP_LEFT:
	case PART_PIPE_UP_RIGHT:
	case PART_PIPE_DOWN_LEFT:
	case PART_PIPE_DOWN_RIGHT:
	case PART_QUESTION0:
	case PART_QUESTION1:
	case PART_QUESTION2:
	case PART_QUESTION3:
	//case PART_CLOUD_UP_LEFT:
	//case PART_CLOUD_UP:
	//case PART_CLOUD_UP_RIGHT:
	//case PART_CLOUD_DOWN_LEFT:
	//case PART_CLOUD_DOWN:
	//case PART_CLOUD_DOWN_RIGHT:
	//case PART_TREE_LEFT:
	//case PART_TREE_CENTER:
	//case PART_TREE_RIGHT:
	//case PART_MOUNTAIN_TOP:
	//case PART_MOUNTAIN_LEFT:
	//case PART_MOUNTAIN_CENTER0:
	//case PART_MOUNTAIN_RIGHT:
	//case PART_MOUNTAIN_PLANE:
	//case PART_MOUNTAIN_CENTER1:
	//case PART_GOAL_TOP:
	//case PART_GOAL_POLE:
	//case PART_FORT_TOP:
	//case PART_FORT_LEFT:
	//case PART_FORT_WALL:
	//case PART_FORT_RIGHT:
	//case PART_FORT_MIDDLE:
	//case PART_FORT_GATE_UP:
	//case PART_FORT_GATE_DOWN:
		return true;
	}
	return false;
}

bool Course::intersect(glm::vec2 const& _point, int* _parts)
{
	ivec2 cellPoint = (ivec2)_point / PART_SIZE;
	*_parts = PART_NONE;
	bool result = false;
	if (
		(cellPoint.x < 0)
		|| (cellPoint.x >= m_width)
		|| (cellPoint.y < 0)
		|| (cellPoint.y >= m_height)
		) {
		return false;
	}

	switch (m_parts[cellPoint.y][cellPoint.x]) {
	case PART_GROUND:
		*_parts = PART_GROUND;
		result = true;
		break;
	case PART_HARD_BLOCK:
		*_parts = PART_HARD_BLOCK;
		result = true;
		break;
	case PART_HARD_BLOCK_2:
		*_parts = PART_HARD_BLOCK_2;
		result = true;
		break;
	case PART_SOFT_BLOCK:
		*_parts = PART_SOFT_BLOCK;
		result = true;
		break;
	case PART_PIPE_UP_LEFT:
		*_parts = PART_PIPE_UP_LEFT;
		result = true;
		break;
	case PART_PIPE_UP_RIGHT:
		*_parts = PART_PIPE_UP_RIGHT;
		result = true;
		break;
	case PART_PIPE_DOWN_LEFT:
		*_parts = PART_PIPE_DOWN_LEFT;
		result = true;
		break;
	case PART_PIPE_DOWN_RIGHT:
		*_parts = PART_PIPE_DOWN_RIGHT;
		result = true;
		break;
	case PART_QUESTION0:
		*_parts = PART_QUESTION0;
		result = true;
		break;
	case PART_QUESTION1:
		*_parts = PART_QUESTION1;
		result = true;
		break;
	case PART_QUESTION2:
		*_parts = PART_QUESTION2;
		result = true;
		break;
	case PART_QUESTION3:
		*_parts = PART_QUESTION3;
		result = true;
		break;
	}

	return result;
}