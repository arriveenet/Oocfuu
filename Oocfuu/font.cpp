#include "font.h"
#include "Header.h"

#include <stdio.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <glm/glm.hpp>
#include <time.h>

using namespace glm;

// Font texture
//static Texture m_texture;
static GLuint texture;
static vec2 textureSize;

// Font Internal variables
static GLint lastMatrixMode;
static ivec2 screenSize;
static vec2 size;
static float scale;
static vec2 m_position;
static vec2 origin;

static GLfloat vertex[8];
static GLfloat texCoord[8];

static GLubyte indices[6] = {
	0, 1, 2, 0, 2, 3
};

int fontInit()
{
	screenSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	textureSize = { 128, 128 };
	scale = 1.0f;
	size = { 8,8 };

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	texFromBMP("resource\\textures\\sprite\\CHR000.bmp", 0, 64, 128);
	//m_texture.loadBitmapFile("font/sprite.bmp", 0, 64, 128);

	return 0;
}

void fontRelease()
{
	glDeleteTextures(1, &texture); // GLsizei n, const GLuint *textures
}

void fontBegin()
{
	glGetIntegerv(
		GL_MATRIX_MODE,		// GLenum pname
		&lastMatrixMode);	// GLint *params

	glPushAttrib(GL_ALL_ATTRIB_BITS);	//GLbitfield mask

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(
		0, screenSize.x,
		screenSize.y, 0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);	//GLenum cap
	glDisable(GL_LIGHTING);		//GLenum cap

	// Enable texture
	glEnable(GL_TEXTURE_2D);	// GLenum cap
	glEnable(GL_BLEND);			// GLenum cap
	glEnableClientState(GL_VERTEX_ARRAY);// GLenum array
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);// GLenum array
	glBlendFunc(
		GL_SRC_ALPHA,			//GLenum sfactor
		GL_ONE_MINUS_SRC_ALPHA);//GLenum dfactor

	glBindTexture(
		GL_TEXTURE_2D,	// GLenum target
		texture);			// GLuint texture
}

void fontEnd()
{
	glBindTexture(
		GL_TEXTURE_2D,	// GLenum target
		0);				// GLuint texture
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(lastMatrixMode);

	glPopAttrib();
}

void fontPosition(float _x, float _y)
{
	origin = m_position = { _x, _y };
}

void fontScale(float _scale)
{
	scale = _scale;
}


void fontDraw(const char* format, ...)
{
	va_list ap;
	char str[256];
	char* p;
	vec2 pos = m_position;

	va_start(ap, format);
	vsprintf_s(str, format, ap);
	va_end(ap);

	for (p = str; (*p != '\0') && (*p != '\n'); p++) {
		int x = (*p % 16) * 8;
		int y = (*p / 16) * 8;

		float leftX = (float)x / (float)textureSize.x;
		float leftY = (float)y / (float)textureSize.y;
		float rightX = (float)(x + 8) / (float)textureSize.x;
		float rightY = (float)(y + 8) / (float)textureSize.y;

		vertex[0] = pos.x * scale;// Upper left x
		vertex[1] = pos.y * scale;// Upper left y
		vertex[2] = pos.x * scale;// Lower left x
		vertex[3] = pos.y + size.y * scale;// Lower left y
		vertex[4] = pos.x + size.x * scale;// Lower right x
		vertex[5] = pos.y + size.y * scale;// Lower right y
		vertex[6] = pos.x + size.x * scale;// Upper right x
		vertex[7] = pos.y * scale;// Upper right y

		texCoord[0] = leftX; // Upper left x
		texCoord[1] = leftY; // Upper left y
		texCoord[2] = leftX; // Lower left x
		texCoord[3] = rightY;// Lower left y
		texCoord[4] = rightX;// Lower right x
		texCoord[5] = rightY;// Lower right y
		texCoord[6] = rightX;// Upper right x
		texCoord[7] = leftY; // Upper right y

		glVertexPointer(
			2,			// GLint size
			GL_FLOAT,	// GLenum type
			0,			// GLsizei stride
			vertex);	// const GLvoid * pointer
		glTexCoordPointer(
			2,			// GLint size
			GL_FLOAT,	// GLenum type
			0,			// GLsizei stride
			texCoord);	// const GLvoid * pointer
		glDrawElements(
			GL_TRIANGLES,		// GLenum mode
			6,					// GLsizei count
			GL_UNSIGNED_BYTE,	// GLenum type
			indices);			// const GLvoid *indices

		pos.x += 8;
	}

	if (*p == '\n') {
		m_position.x = origin.x;
		m_position.y += 8;
		fontDraw(++p);
	}
}