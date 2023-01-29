#pragma once
#include "Game.h"
#include "Rect.h"

#include <gl/glut.h>
#include <glm/glm.hpp>
#include <Windows.h>

#include <vector>
#include <string>

// �R�[�X�̓ǂݍ��ݎ��̃G���[��񋓂���
enum COURSE_ERROR {
	COURSE_NO_ERROR = 0,		// �G���[�Ȃ�
	COURSE_INVALID_VALUE,		// �����ȃp�[�c���w�肳��Ă���
	COURSE_INVALID_SIZE,		// �R�[�X�̃T�C�Y��0�ȉ�
	COURSE_FAILED_OPEN_FILE,	// �R�[�X�t�@�C�����J���Ȃ�����
	COURSE_OUT_OF_MEMORY,		// �R�[�X�f�[�^��ǂݍ��ރ��������c���Ă��Ȃ�
};

// �\���̂��`����
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
	Rect m_clearAex;
	COURSE_ERROR m_courseError;
	std::string m_errorMsg;

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
	bool getClearAex(Rect& _rect);
	COURSE_ERROR getError() const;
	std::string getErrorString() const;
};

extern CourseManager g_courseManager;