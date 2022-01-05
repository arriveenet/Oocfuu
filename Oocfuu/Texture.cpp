#include <stdio.h>
#include "Texture.h"
#include "tex.h"

#include <glut.h>

Texture g_textures[TEXTURE_MAX] = {
	{"Player\\BigMario_Brake.bmp"},
	{"Player\\BigMario_Crouch.bmp"},
	{"Player\\BigMario_Die.bmp"},
	{"Player\\BigMario_Flagpole_1.bmp"},
	{"Player\\BigMario_Flagpole_2.bmp"},
	{"Player\\BigMario_Idle.bmp"},
	{"Player\\BigMario_Jump.bmp"},
	{"Player\\BigMario_Run_1.bmp"},
	{"Player\\BigMario_Run_2.bmp"},
	{"Player\\BigMario_Run_3.bmp"},
	{"Player\\SmallMario_Brake.bmp"},
	{"Player\\SmallMario_Die.bmp"},
	{"Player\\SmallMario_Flagpole_1.bmp"},
	{"Player\\SmallMario_Flagpole_2.bmp"},
	{"Player\\SmallMario_Idle.bmp"},
	{"Player\\SmallMario_Jump.bmp"},
	{"Player\\SmallMario_Run_1.bmp"},
	{"Player\\SmallMario_Run_2.bmp"},
	{"Player\\SmallMario_Run_3.bmp"},
	{"Enemies\\Kuribo_Run.bmp"},
	{"Enemies\\Kuribo_Squish.bmp"},
	{"Enemies\\Nokonoko_Run_1.bmp"},
	{"Enemies\\Nokonoko_Run_2.bmp"},
	{"Enemies\\Nokonoko_Squish.bmp"},
	{"Enemies\\Nokonoko_Return.bmp"},
};

int Texture::init()
{
	glGenTextures(
		1,				// GLsizei n
		&m_texture);	// GLuint *textures

	glBindTexture(
		GL_TEXTURE_2D,	// GLenum target
		m_texture);	// GLuint texture
	char fileName[256] = "Textures\\";
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