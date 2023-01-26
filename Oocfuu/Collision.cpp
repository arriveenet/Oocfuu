#include "Collision.h"
#include "world/Part.h"
#include "world/Course.h"

#include <gl/glut.h>

using namespace glm;
using namespace std;

Collision::Collision()
{
}

void Collision::clear()
{
	m_leftPoints.clear();
	m_rightPoints.clear();
	m_topPoints.clear();
	m_bottomPoints.clear();
}

void Collision::prepare(const Rect& _rect)
{
	clear();
	
	m_rightPoints.push_back(_rect.m_position);
}

template<typename TUpdate>
inline void Collision::updateLeft(TUpdate _update, CourseManager _course)
{
	vector<vec2>::iterator iter = m_leftPoints.begin();
	for (; iter != m_leftPoints.end(); iter++) {
		if (_course.intersect(iter)) {

		}
		_update();
	}
}

template<typename TUpdate>
inline void Collision::updateRight(TUpdate _update, CourseManager _course)
{
	vector<vec2>::iterator iter = m_rightPoints.begin();
	for (; iter != m_rightPoints.end(); iter++) {
		if (_course.intersect(iter)) {

		}
		_update();
	}
}

template<typename TUpdate>
inline void Collision::updateTop(TUpdate _update, CourseManager _course)
{
	vector<vec2>::iterator iter = m_topPoints.begin();
	for (; iter != m_topPoints.end(); iter++) {
		if (_course.intersect(iter)) {
		
		}
		_update();
	}
}

template<typename TUpdate>
inline void Collision::updateBottom(TUpdate _update, CourseManager _course)
{
	vector<vec2>::iterator iter = m_bottomPoints.begin();
	for (; iter != m_bottomPoints.end(); iter++) {
		if (_course.intersect(iter)) {

		}
		_update();
	}
}

void Collision::drawPoints()
{
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);// GLbitfield mask
	glPushAttrib(GL_ALL_ATTRIB_BITS);// GLbitfield mask
	glDisable(GL_TEXTURE_2D);// GLenum cap
	glPointSize(4.0f);
	glColor3ub(0xff, 0x00, 0x00);
	glEnableClientState(GL_VERTEX_ARRAY);// GLenum array
	glVertexPointer(
		2,						// GLint size
		GL_FLOAT,				// GLenum type
		0,						// GLsizei stride
		m_rightPoints.data());	// const GLvoid * pointer
	glDrawArrays(
		GL_POINTS,				// GLenum mode
		0,						// GLint first
		(GLsizei)m_rightPoints.size());	// GLsizei count

	glVertexPointer(
		2,						// GLint size
		GL_FLOAT,				// GLenum type
		0,						// GLsizei stride
		m_leftPoints.data());	// const GLvoid * pointer
	glDrawArrays(
		GL_POINTS,				// GLenum mode
		0,						// GLint first
		(GLsizei)m_leftPoints.size());	// GLsizei count

	glColor3ub(0x00, 0x80, 0x00);
	glVertexPointer(
		2,						// GLint size
		GL_FLOAT,				// GLenum type
		0,						// GLsizei stride
		m_bottomPoints.data());	// const GLvoid * pointer
	glDrawArrays(
		GL_POINTS,				// GLenum mode
		0,						// GLint first
		(GLsizei)m_bottomPoints.size());	// GLsizei count

	glColor3ub(0x00, 0x00, 0xff);
	glVertexPointer(
		2,						// GLint size
		GL_FLOAT,				// GLenum type
		0,						// GLsizei stride
		m_topPoints.data());	// const GLvoid * pointer
	glDrawArrays(
		GL_POINTS,				// GLenum mode
		0,						// GLint first
		(GLsizei)m_topPoints.size());	// GLsizei count

	glPopAttrib();
	glPopClientAttrib();
}
