#include "TextureManager.h"
#include "Texture.h"

#include <stdio.h>

TextureManager g_textureManager;

static const char* filePath = "resource\\textures\\";

static TEX texs[] =
{
	{{"player\\Player_Idle.bmp"}, {0xff, 0x00, 0xff}},	// TEXTURE_PLAYER_IDLE
	{{"player\\Player_Jump.bmp"}, {0xff, 0x00, 0xff}},	// TEXTURE_PLAYER_JUMP
	{{"player\\Player_Run.bmp"}, {0xff, 0x00, 0xff}},	// TEXTURE_PLAYER_RUN_1
	{{"player\\Player_Idle.bmp"}, {0xff, 0x00, 0xff}},	// TEXTURE_PLAYER_RUN_2
	{{"enemy\\Kuribo_Run.bmp"}, {0xff, 0x00, 0xff}},	// TEXTURE_PLAYER_RUN_1
	{{"enemy\\Kuribo_Squish.bmp"}, {0xff, 0x00, 0xff}},	// TEXTURE_KURIBO_SQUISH
	{{"item\\cake.bmp"}, {0xff, 0x00, 0xff}},			// TEXTURE_CAKE
	{{"item\\coin_1.bmp"}, {0xff, 0x00, 0xff}},			// TEXTURE_COIN_1
	{{"item\\firework_1.bmp"}, {0xff, 0x00, 0xff}},		// TEXTURE_FIREWORK_1
	{{"item\\firework_2.bmp"}, {0xff, 0x00, 0xff}},		// TEXTURE_FIREWORK_2
	{{"item\\firework_3.bmp"}, {0xff, 0x00, 0xff}},		// TEXTURE_FIREWORK_3
	{{"item\\title_2.bmp"}, {0xff, 0x00, 0xff}},		// TEXTURE_TITLE
	{{"parts\\part.bmp"}, {0xff, 0x00, 0xff}},			// TEXTURE_PARTS
	{{"parts\\fireball.bmp"}, {0xff, 0x00, 0xff}},		// TEXTURE_FIREBALL
	{{"parts\\lift.bmp"}, {0xff, 0x00, 0xff}},			// TEXTURE_LIFT
	{{"font\\CHR000.bmp"}, {0, 64, 128}},				// TEXTURE_FONT
	{{"enemy\\Koopa_Idle_1.bmp"}, {0xff, 0x00, 0xff}},	// TEXTURE_KOOPA_IDLE_1
	{{"enemy\\Koopa_Idle_2.bmp"}, {0xff, 0x00, 0xff}},	// TEXTURE_KOOPA_IDLE_2
	{{"enemy\\Koopa_Idle_Breath_1.bmp"}, {0xff, 0x00, 0xff}},// TEXTURE_KOOPA_BREATH_1
	{{"enemy\\Koopa_Idle_Breath_2.bmp"}, {0xff, 0x00, 0xff}},// TEXTURE_KOOPA_BREATH_2
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