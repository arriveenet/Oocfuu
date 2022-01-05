#include <stdio.h>
#include <stdarg.h>

#include "font.h"
#include "Sprite.h"
#include "Rect.h"

#include <glut.h>

using namespace glm;

static vec2 size = { FONT_DEFAULT_WIDTH, FONT_DEFAULT_HEIGHT };
static vec2 screenSize = { 256, 240 };
static vec2 position;
static vec2 origin;
static float weight = 1;
static GLint lastMatrixMode;

void fontInit(float _width, float _height) {
	fontScreenSize(_width, _height);
	size = { 8, 8 };
}

void fontScreenSize(float _width, float _height) {
	screenSize = vec2(_width, _height);
}

void fontBegin() {
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

	glDisable(GL_DEPTH_TEST);//GLenum cap
	glDisable(GL_LIGHTING);	//GLenum cap
	glEnable(GL_TEXTURE_2D);//GLenum cap
}

void fontEnd() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(lastMatrixMode);

	glPopAttrib();
}

void fontPosition(float _x, float _y) {
	fontPosition(vec2(_x, _y));
}

void fontPosition(vec2 const& _position) {
	origin = position = _position;
}

void fontDraw(const char* _format, ...) {
	va_list argList;
	va_start(argList, _format);
	char str[256];
	vsprintf_s(str, _format, argList);
	va_end(argList);

	char* p = str;

	for (; (*p != '\0') && (*p != '\n'); p++) {

		glBindTexture(
			GL_TEXTURE_2D,		// GLenum target
			g_sprite.m_textures[*p]);	// GLuint texture
		Rect(size, position).draw();
		position.x += size.x;
	}

	if (*p == '\n') {
		position.x = origin.x;
		position.y += size.y;
		fontDraw(++p);
	}

	//printf("%s\n", str);
}