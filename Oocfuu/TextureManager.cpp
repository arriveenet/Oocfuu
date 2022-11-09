#include "TextureManager.h"
#include "Texture.h"

#include <stdio.h>

TextureManager g_textureManager;

static const char* filePath = "resource\\textures\\";

static TEX texs[] =
{
	{{"player\\Player_Idle.bmp"}, {0xff, 0x00, 0xff}},
	{{"player\\Player_Jump.bmp"}, {0xff, 0x00, 0xff}},
	{{"player\\Player_Run.bmp"}, {0xff, 0x00, 0xff}},
	{{"player\\Player_Idle.bmp"}, {0xff, 0x00, 0xff}},
	{{"item\\cake.bmp"}, {0xff, 0x00, 0xff}},
	{{"item\\coin_1.bmp"}, {0xff, 0x00, 0xff}},
	{{"item\\firework_1.bmp"}, {0xff, 0x00, 0xff}},
	{{"item\\firework_2.bmp"}, {0xff, 0x00, 0xff}},
	{{"item\\firework_3.bmp"}, {0xff, 0x00, 0xff}},
	{{"item\\title_2.bmp"}, {0xff, 0x00, 0xff}},
	{{"parts\\part.bmp"}, {0xff, 0x00, 0xff}},
	{{"font\\CHR000.bmp"}, {0, 64, 128}},
};

TextureManager::TextureManager()
	: m_textures{ 0 }
{
}

TextureManager::~TextureManager()
{
	release();
}

bool TextureManager::init()
{
	glGenTextures(TEXTURE_MAX, m_textures);

	for (int i = 0; i < TEXTURE_MAX; i++) {
		glBindTexture(
			GL_TEXTURE_2D,	// GLenum target
			m_textures[i]);	// GLuint texture

		Texture tex;
		char fileName[256] = { 0 };

		sprintf_s(fileName, "%s%s", filePath, texs[i].fileName);

		if (tex.loadBitmapFile(fileName, texs[i].colorKey.r, texs[i].colorKey.g, texs[i].colorKey.b) == 0) {
			glTexImage2D(
				GL_TEXTURE_2D,		// GLenum target
				0,					// GLint level
				GL_RGBA,			// GLint internalformat
				tex.getWidth(),		// GLsizei width
				tex.getHeight(),	// GLsizei height
				0,					// GLint border
				GL_RGBA,			// GLenum format
				GL_UNSIGNED_BYTE,	// GLenum type
				tex.getTexImage());	// const GLvoid *pixels
			glTexParameteri(
				GL_TEXTURE_2D,			// GLenum target
				GL_TEXTURE_MAG_FILTER,	// GLenum pname
				GL_NEAREST);			// GLint param
			glTexParameteri(
				GL_TEXTURE_2D,			// GLenum target
				GL_TEXTURE_MIN_FILTER,	// GLenum pname
				GL_NEAREST);			// GLint param
			glEnable(GL_TEXTURE_2D);		
		tex.deleteTexImage();

		glBindTexture(
			GL_TEXTURE_2D,	// GLenum target
			0);				// GLuint texture
		} else {
			printf("%s file load failed.\n", fileName);
			return false;
		}
	}

	return true;
}

void TextureManager::release()
{
	glDeleteTextures(TEXTURE_MAX, m_textures);
}

void TextureManager::setTexture(enum TEXTURE _tex)
{
	glBindTexture(
		GL_TEXTURE_2D,		// GLenum target
		m_textures[_tex]);	// GLuint texture
}

GLuint TextureManager::getTexture(enum TEXTURE _tex)
{
	return m_textures[_tex];
}

void TextureManager::unbindTexture()
{
	glBindTexture(
		GL_TEXTURE_2D,	// GLenum target
		0);				// GLuint texture
}