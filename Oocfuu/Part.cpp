#include <stdio.h>
#include "Part.h"
#include "tex.h"

#include "glut.h"

Part g_parts[PART_MAX] = {
	 {nullptr,}, // PART_NONE
	 {"b3.bmp",},// PART_HARD_BLOCK_2
};

int Part::init() {
	if (m_fileName != nullptr) {
		glGenTextures(
			1,				// GLsizei n
			&m_texture);	// GLuint * textures

		glBindTexture(
			GL_TEXTURE_2D,	// GLenum target
			m_texture);		// GLuint texture
		char fileName[256] = "resource\\textures\\parts\\";
		sprintf_s(fileName, "%s%s", fileName, m_fileName);
		//printf("%s\n", fileName);
		texFromBMP(fileName, 0xff, 0x00, 0xff);
	}
	return 0;
}

int Part::initAll() {
	for (int i = 0; i < PART_MAX; i++)
		g_parts[i].init();
	return 0;
}
