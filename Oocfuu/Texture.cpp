#include <stdio.h>
#include "Texture.h"
#include "tex.h"

#include <glut.h>

Texture g_textures[TEXTURE_MAX] = {
	{"player\\SmallMario_Brake.bmp"},
	{"player\\SmallMario_Die.bmp"},
	{"player\\SmallMario_Flagpole_1.bmp"},
	{"player\\SmallMario_Flagpole_2.bmp"},
	{"player\\SmallMario_Idle.bmp"},
	{"player\\SmallMario_Jump.bmp"},
	{"player\\SmallMario_Run_1.bmp"},
	{"player\\SmallMario_Run_2.bmp"},
	{"player\\SmallMario_Run_3.bmp"},
	{"cake.bmp"},
	{"coin_1.bmp"},
	{"coin_2.bmp"},
	{"coin_3.bmp"},
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