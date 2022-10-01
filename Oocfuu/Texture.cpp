#include <stdio.h>
#include "Texture.h"
#include "tex.h"

#include <glut.h>

Texture g_textures[TEXTURE_MAX] = {
	{"player\\Player_Idle.bmp"},
	{"player\\Player_Jump.bmp"},
	{"player\\Player_Run.bmp"},
	{"player\\Player_Idle.bmp"},
	{"item\\cake.bmp"},
	{"item\\coin_1.bmp"},
	{"item\\firework_1.bmp"},
	{"item\\firework_2.bmp"},
	{"item\\firework_3.bmp"},
	{"parts\\part.bmp"},
};

int Texture::init()
{
	glGenTextures(
		1,				// GLsizei n
		&m_texture);	// GLuint *textures

	glBindTexture(
		GL_TEXTURE_2D,	// GLenum target
		m_texture);	// GLuint texture
	char fileName[256] = "resource\\textures\\";
	sprintf_s(fileName, "%s%s", fileName, m_fileName);
	//printf("%s\n", fileName);
	texFromBMP(fileName, 0xff, 0x00, 0xff);

	return 0;
}

int Texture::initAll()
{
	for (int i = 0; i < TEXTURE_MAX; i++)
		g_textures[i].init();

	return 0;
}