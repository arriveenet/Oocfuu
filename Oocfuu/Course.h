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
	unsigned short width;		// コースの幅
	unsigned short height;		// コースの高さ
	COLORREF clearColor;		// バッファのクリアカラー
	unsigned short startX;		// プレイヤーのスタートX座標
	unsigned short startY;		// プレイヤーのスタートY座標
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