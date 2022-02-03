#pragma once

#include <glm/vec2.hpp>

#define SPRITE_SIZE		256
#define SPRITE_WIDTH	8
#define SPRITE_HEIGHT	8

class Sprite {
public:
	unsigned int m_textures[SPRITE_SIZE];

	int loadBMPFile(const char* _fileName, unsigned char* _colorKey = nullptr);
	int loadBMPFile(const char* _fileName,
		unsigned char _colorKeyR,
		unsigned char _colorKeyG,
		unsigned char _colorKeyB);
private:
	void inline bindTexture(int _width, int _heihgt, void* _pixels);
};

extern Sprite g_sprite;