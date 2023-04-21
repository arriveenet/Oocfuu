#pragma once
#include <glm/glm.hpp>

#define SCREEN_WIDTH	256
#define SCREEN_HEIGHT	240

// ç\ë¢ëÃÇíËã`Ç∑ÇÈ
typedef struct {
	glm::vec2 position;
	glm::vec2 texCoord;
}VERTEX;

typedef struct {
	VERTEX vertices[3];
}TRIANGLE;

typedef struct {
	VERTEX vertices[4];
}QUAD;
