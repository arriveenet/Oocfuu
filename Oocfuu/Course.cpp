#include <stdio.h>
#include <string.h>

#include "Course.h"
#include "Part.h"
#include "Rect.h"
#include "Header.h"

using namespace glm;
using std::vector;

Course g_course{ COURSE_WIDTH, COURSE_HEIGHT };

Course::Course(int _width, int _height)
	: m_width(_width)
	, m_height(_height)
	, m_scroll(0.0f)
	, m_lastScroll(0.0f)
	, m_texture(0)
{
	m_parts = new int* [m_height];
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

	glDeleteTextures(1, &m_texture);
}

int Course::load(const char* _fileName) {
	FILE* pFile;
	errno_t error;

	error = fopen_s(&pFile, _fileName, "r");
	if (error != 0) {
		printf("The file %s was open failed!\n", _fileName);
		return 1;
	}
	printf("The file %s was opened!\n", _fileName);

	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			char buf[2];
			fread(buf, sizeof(char), 2, pFile);
			//printf("%d-%d %c%c", i, j, buf[0], buf[1]);
			if (buf[0] == 0x20) {
				m_parts[i][j] = PART_NONE;
			}
			else
				for (int k = PART_NONE + 1; k < PART_MAX; k++) {
					if (strncmp(buf, g_parts[k].m_fileName, 2) == 0) {
						m_parts[i][j] = k;
						break;
					}
				}
		}
		fseek(pFile, 2, SEEK_CUR);
	}
	fclose(pFile);

	update();

	return 0;
}

int Course::reload(const char* _fileName, int _width, int _height)
{
	for (int i = 0; i < m_height; ++i) {
		delete m_parts[i];
	}
	delete[] m_parts;

	m_width = _width;
	m_height = _height;

	m_parts = new int* [m_height];
	for (int i = 0; i < m_height; i++) {
		m_parts[i] = new int[m_width];
	}

	m_scroll = 0.0f;
	return load(_fileName);
}

void Course::update()
{
	//if ((m_scroll == m_lastScroll) && (m_scroll != 0.0f))
	//	return;

	m_quads.clear();

	int scrolleColumn = (int)m_scroll / PART_SIZE;
	//printf("scrolleColumn=%d\n", scrolleColumn);
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			int part = m_parts[y][x];
			if (
				(x < scrolleColumn)
				|| (x > scrolleColumn + (SCREEN_WIDTH / PART_SIZE))
				|| (part == PART_NONE)
				)
				continue;

			int textureIndex = part;
			switch (part) {
			case PART_QUESTION0:
			{
				int animationTable[] = { 0,1,2,2,1,0 };
				int animationTableLength = sizeof(animationTable) / sizeof(int);
				textureIndex += animationTable[(g_game.m_count / 8) % animationTableLength];
			}
			break;
			case PART_SEA_0:
			{
				int animationTable[] = { 0,1,2,3,4,5,6,7 };
				int animationTableLength = sizeof(animationTable) / sizeof(int);
				textureIndex += animationTable[(g_game.m_count / 16) % animationTableLength];
			}
			break;
			case PART_DESERT_0:
			{
				int animationTable[] = { 0,1,2,3,4,5,6,7 };
				int animationTableLength = sizeof(animationTable) / sizeof(int);
				textureIndex += animationTable[(g_game.m_count / 16) % animationTableLength];
			}
			break;
			}
			textureIndex--;

			float x2 = (float)x * PART_SIZE;
			float y2 = (float)y * PART_SIZE;

			float tx0 = (float)g_parts[textureIndex].m_uvX / 256;
			float tx1 = (float)g_parts[textureIndex].m_sizeX / 256;
			float ty0 = (float)g_parts[textureIndex].m_uvY / 256;
			float ty1 = (float)g_parts[textureIndex].m_sizeY / 256;

			QUAD quad;
			quad.vertices[0].position = { x2, y2 };
			quad.vertices[0].texCoord = { tx0, ty0 };
			quad.vertices[1].position = { x2 + PART_SIZE,y2 };
			quad.vertices[1].texCoord = { tx1, ty0 };
			quad.vertices[2].position = { x2 + PART_SIZE, y2 + PART_SIZE };
			quad.vertices[2].texCoord = { tx1, ty1 };
			quad.vertices[3].position = { x2, y2 + PART_SIZE };
			quad.vertices[3].texCoord = { tx0, ty1 };
			m_quads.push_back(quad);
		}
	}

	m_lastScroll = m_scroll;
}

void Course::draw() {
	if (m_quads.empty())
		return;

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(2, GL_FLOAT, sizeof(VERTEX), &m_quads[0].vertices[0].position);
	glTexCoordPointer(2, GL_FLOAT, sizeof(VERTEX), &m_quads[0].vertices[0].texCoord);

	glBindTexture(GL_TEXTURE_2D, g_textures[TEXTURE_PARTS].m_texture);
	glDrawArrays(GL_QUADS, 0, GLsizei(m_quads.size() * 4));
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
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
	case PART_SOFT_BLOCK:
	case PART_PIPE_UP_LEFT:
	case PART_PIPE_UP_RIGHT:
	case PART_PIPE_DOWN_LEFT:
	case PART_PIPE_DOWN_RIGHT:
	case PART_QUESTION0:
	case PART_QUESTION1:
	case PART_QUESTION2:
	case PART_QUESTION3:
	case PART_GROUND_2:
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
	case PART_GROUND_2:
		*_parts = PART_GROUND_2;
		result = true;
		break;
	}

	return result;
}

int Course::getWidth()
{
	return m_width;
}

int Course::getHeight()
{
	return m_height;
}