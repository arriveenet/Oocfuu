#include "Rect.h"

#include <gl/glut.h>

using namespace glm;

Rect::Rect()
	:m_size(0)
	, m_position(0, 0)
	, m_flip(RECT_FLIP_NONE)
{}

Rect::Rect(float _width, float _height)
	: m_size(_width, _height)
	, m_position(vec2())
	, m_flip(RECT_FLIP_NONE)
{}

Rect::Rect(vec2 const& _size)
	: m_size(_size)
	, m_position(vec2())
	, m_flip(RECT_FLIP_NONE)
{};

Rect::Rect(vec2 const& _size, vec2 const& _position)
	:m_size(_size)
	, m_position(_position)
	, m_flip(RECT_FLIP_NONE)
{};

void Rect::draw() {
	/* glRectf(
		m_position.x,			 //GLfloat x1
		m_position.y,			 // GLfloat y1,
		m_position.x + m_size.x, //GLfloat x2
		m_position.y + m_size.y  //GLfloat y2
	);
	*/
	glBegin(GL_QUADS);
	{
		vec2 upperLeft = m_position;
		vec2 lowLeft = m_position + vec2(0, m_size.y);
		vec2 lowerRight = m_position + m_size;
		vec2 upperRight = m_position + vec2(m_size.x, 0);

		glTexCoord2f(
			(m_flip & RECT_FLIP_HORIZONTAL) ? (GLfloat)1 : (GLfloat)0,	// GLfloat s
			(m_flip & RECT_FLIP_VERTICAL) ? (GLfloat)1 : (GLfloat)0);	// GLfloat t
		glVertex2fv((GLfloat*)&upperLeft);								// const GLfloat *v

		glTexCoord2f(
			(m_flip & RECT_FLIP_HORIZONTAL) ? (GLfloat)1 : (GLfloat)0,	// GLfloat s
			(m_flip & RECT_FLIP_VERTICAL) ? (GLfloat)0 : (GLfloat)1);	// GLfloat t
		glVertex2fv((GLfloat*)&lowLeft);								// const GLfloat *v

		glTexCoord2f(
			(m_flip & RECT_FLIP_HORIZONTAL) ? (GLfloat)0 : (GLfloat)1,	// GLfloat s
			(m_flip & RECT_FLIP_VERTICAL) ? (GLfloat)0 : (GLfloat)1);	// GLfloat t
		glVertex2fv((GLfloat*)&lowerRight);								// const GLfloat *v

		glTexCoord2f(
			(m_flip & RECT_FLIP_HORIZONTAL) ? (GLfloat)0 : (GLfloat)1,	// GLfloat s
			(m_flip & RECT_FLIP_VERTICAL) ? (GLfloat)1 : (GLfloat)0);	// GLfloat t
		glVertex2fv((GLfloat*)&upperRight);								// const GLfloat *v
	}
	glEnd();
}

void Rect::drawWire()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(
		m_position.x,				// GLfloat x1
		m_position.y,				// GLfloat y1
		m_position.x + m_size.x,	// GLfloat x2
		m_position.y + m_size.y);	// GLfloat y2
	glPopAttrib();
}

bool Rect::intersect(vec2 const& _point) {
	if (
		(_point.x >= m_position.x)
		&& (_point.x < m_position.x + m_size.x)
		&& (_point.y >= m_position.y)
		&& (_point.y < m_position.y + m_size.y)
		)
		return true;

	return false;
};

bool Rect::intersect(Rect const& _rect) {
	if (
		(m_position.x + m_size.x >= _rect.m_position.x)
		&& (m_position.x < _rect.m_position.x + _rect.m_size.x)
		&& (m_position.y + m_size.y >= _rect.m_position.y)
		&& (m_position.y < _rect.m_position.y + _rect.m_size.y)
		)
		return true;

	return false;
}

bool Rect::operator==(const Rect& _rect)
{
	if ((m_position == _rect.m_position) && (m_size == _rect.m_size))
		return true;

	return false;
}

bool Rect::operator!=(const Rect& _rect)
{
	if ((m_position != _rect.m_position) || (m_size != _rect.m_size))
		return true;

	return false;
}
;