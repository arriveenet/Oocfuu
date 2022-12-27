#pragma once

#include <vector>
#include <Windows.h>

#include <gl/glut.h>
#include <glm/glm.hpp>

typedef struct {
	glm::vec2 position;
	glm::vec2 texCoord;
}VERTEX;

typedef struct {
	VERTEX vertices[4];
}QUAD;

typedef struct _COURSEHEADER {
	unsigned short width;		// �R�[�X�̕�
	unsigned short height;		// �R�[�X�̍���
	COLORREF clearColor;		// �o�b�t�@�̃N���A�J���[
	unsigned short startX;		// �v���C���[�̃X�^�[�gX���W
	unsigned short startY;		// �v���C���[�̃X�^�[�gY���W
} COURSEHEADER;

class CourseManager {
	int m_width;
	int m_height;
	int** m_pParts;
	std::vector<QUAD> m_quads;

public:
	float m_scroll;

	CourseManager();
	~CourseManager();

	bool init(int _width, int _height);
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
};

extern CourseManager g_courseManager;