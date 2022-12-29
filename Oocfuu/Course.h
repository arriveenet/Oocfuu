#pragma once

#include <vector>
#include <Windows.h>

#include <gl/glut.h>
#include <glm/glm.hpp>

#include "Game.h"

typedef struct {
	glm::vec2 position;
	glm::vec2 texCoord;
}VERTEX;

typedef struct {
	VERTEX vertices[4];
}QUAD;

typedef struct COURSEINFOHEADER {
	unsigned short width;		// �R�[�X�̕�
	unsigned short height;		// �R�[�X�̍���
	COLORREF clearColor;		// �o�b�t�@�̃N���A�J���[
	unsigned short startX;		// �v���C���[�̃X�^�[�gX���W
	unsigned short startY;		// �v���C���[�̃X�^�[�gY���W
	BYTE nextWorld;
	BYTE nextStage;
} COURSEINFOHEADER, *LPCOURSEINFOHEADER;

class CourseManager {
	int m_width;
	int m_height;
	COLORREF m_clearColor;
	glm::ivec2 m_startPosition;
	WORLD m_nextWorld;
	int** m_pParts;
	bool m_isLoaded;
	std::vector<QUAD> m_quads;

public:
	float m_scroll;

	CourseManager();
	~CourseManager();

	void release();
	bool load(const char* _fileName);
	void update();
	void draw();
	void setParts(glm::ivec2 const& _point, int _parts);
	int getParts(int _x, int _y);
	bool intersect(glm::vec2 const& _point);
	bool intersect(glm::vec2 const& _point, int* _parts);
	int getWidth();
	int getHeight();
	COLORREF getClearColor();
	glm::ivec2 getStartPosition();
	WORLD getNextWorld();
};

extern CourseManager g_courseManager;