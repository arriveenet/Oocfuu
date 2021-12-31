#include <stdio.h>
#include <stdarg.h>

#include "font.h"

#include <glut.h>

#define FONT	GLUT_STROKE_MONO_ROMAN

static vec2 size = { FONT_DEFAULT_WIDTH, FONT_DEFAULT_HEIGHT };
static vec2 screenSize = { 256, 256 };
static vec2 position;
static vec2 origin;
static float weight = 1;
static GLint lastMatrixMode;

void fontInit(float _width, float _height) {
	fontScreenSize(_width, _height);
	fontSize(8, 8);
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
	glDisable(GL_TEXTURE_2D);//GLenum cap
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

void fontSize(float _width, float _height) {
	fontSize(vec2(_width, _height));
}

void fontSize(float _size) {
	fontSize(vec2(_size, _size));
}

void fontSize(vec2 const& _size) {
	size = _size;
}

void fontWeight(float _weight) {
	weight = _weight;
}

float fontGetWidth(int _character) {
	return glutStrokeWidth(
		FONT,		//void *font
		_character) //int character
		* size.x / FONT_DEFAULT_WIDTH;
}

float fontGetLength(const unsigned char* _string) {
	return glutStrokeLength(
		FONT,	// void *font
		_string)// const unsigned char *string
		* size.x / FONT_DEFAULT_WIDTH;
}

vec2 fontGetSize() {
	return size;
}

float fontGetWeightMin() {
	GLfloat weight[2];
	glGetFloatv(
		GL_LINE_WIDTH_RANGE,//GLenum pname, 
		weight//GLfloat * params
	);
	return weight[0];
}

float fontGetWeightMax() {
	GLfloat weight[2];
	glGetFloatv(
		GL_LINE_WIDTH_RANGE,//GLenum pname, 
		weight//GLfloat * params
	);
	return weight[1];
}

float fontGetWeight() {
	return weight;
}

void fontDraw(const char* _format, ...) {
	va_list argList;
	va_start(argList, _format);
	char str[256];
	vsprintf_s(str, _format, argList);
	va_end(argList);

	{
		GLint viewport[4];
		glGetIntegerv(
			GL_VIEWPORT,	// GLenum pname
			viewport);// GLint * params
		//printf("%d %d\n", viewport[2], viewport[3]);
		glLineWidth(weight * viewport[3] / screenSize.y);//GLfloat width
	}

	char* p = str;

	for (; (*p != '\0') && (*p != '\n'); p++) {
		glPushMatrix();
		{
			glTranslatef(position.x, position.y + size.y, 0);// GLfloat x, GLfloat y, GLfloat z /
			glScalef(
				size.x / FONT_DEFAULT_WIDTH,	// GLfloat x
				-size.y / FONT_DEFAULT_HEIGHT,	// GLfloat y
				0);								// GLfloat z
			glutStrokeCharacter(
				FONT, //void* font
				*p);  //int character
			position.x += fontGetWidth(*p);
		}
		glPopMatrix();
	}

	if (*p == '\n') {
		position.x = origin.x;
		position.y += fontGetSize().y;
		fontDraw(++p);
	}

	//printf("%s\n", str);
}