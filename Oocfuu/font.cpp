#include "font.h"
#include "App.h"
#include "Course.h"
#include "TextureManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <gl/glut.h>
#include <glm/glm.hpp>
#include <time.h>

using namespace glm;

// Font texture
static vec2 textureSize;

// Font Internal variables
static GLint lastMatrixMode;
static ivec2 screenSize;
static ivec2 size;
static float scale;
static vec2 m_position;
static vec2 origin;

int fontInit()
{
	screenSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	textureSize = { 128, 128 };
	scale = 1.0f;
	size = { 8,8 };

	return 0;
}

void fontRelease()
{

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
	g_textureManager.setTexture(TEXTURE_FONT);
}

void fontEnd()
{
	g_textureManager.unbindTexture();
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

	va_start(ap, format);
	vsprintf_s(str, format, ap);
	va_end(ap);

	int x = (int)m_position.x, y = (int)m_position.y;
	p = str;
	std::vector<QUAD> quads;

	while (1) {
		if ((*p) == '\n') {
			x = (int)m_position.x;
			y += size.y;
			p++;
			if (!(*p)) {
				m_position.y = (float)y;
				break;
			}
			continue;
		}

		int xoffset = (*p % 16) * 8;
		int yoffset = (*p / 16) * 8;
		//printf("xy: %d, %d\n", x, y);
		
		GLfloat tx0 = (float)xoffset / (float)textureSize.x;
		GLfloat tx1 = (float)(xoffset + size.x) / (float)textureSize.x;
		GLfloat ty0 = (float)yoffset / (float)textureSize.y;
		GLfloat ty1 = (float)(yoffset + size.y) / (float)textureSize.y;

		QUAD quad = {
			{
				{{x, y}, {tx0, ty0}},
				{{x , y + size.y}, {tx0, ty1}},
				{{x + size.x, y + size.y}, {tx1, ty1}},
				{{x + size.x, y}, {tx1, ty0}}
			}
		};
		quads.push_back(quad);

		x += size.x;
		p++;

		if (!(*p)) {
			fontPosition(origin.x, (float)y);
			break;
		}
	}

	glVertexPointer(3, GL_FLOAT, sizeof(VERTEX), &quads[0].vertices[0].position);
	glTexCoordPointer(2, GL_FLOAT, sizeof(VERTEX), &quads[0].vertices[0].texCoord);
	glDrawArrays(GL_QUADS, 0, GLsizei(quads.size() * 4));
}