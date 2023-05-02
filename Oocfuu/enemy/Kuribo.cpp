#include "Kuribo.h"

#include "App.h"
#include "world/Part.h"
#include "world/Course.h"
#include "world/CourseEffect.h"
#include "TextureManager.h"
#include "player/Player.h"
#include "sound/Sound.h"

#include <gl/glut.h>

using namespace glm;
using namespace std;

Kuribo::Kuribo()
	: Kuribo(0, 0)
{
}

Kuribo::Kuribo(glm::vec2 _position)
	: Kuribo(_position.x, _position.y)
{
}

Kuribo::Kuribo(float _x, float _y)
	: m_falling(false)
	, m_state(KURIBO_STATE_MOVE)
	, m_counter(0)
	, m_rightPoint(0, 0)
	, m_leftPoint(0, 0)
	, m_topPoints{ vec2(0.0f, 0.0f) }
	, Enemy(vec2(KURIBO_WIDTH, KURIBO_HEIGHT), vec2(_x, _y))
{
	m_pStateMachine = new StateMachine<Kuribo>(this);
	m_pStateMachine->setCurrentState(KuriboStateRun::instance());

	m_speed = { -KURIBO_SPEED, 0 };
}

Kuribo::Kuribo(const Kuribo& _kuribo)
	: Kuribo(_kuribo.m_position)
{
	//m_pStateMachine = new StateMachine<Kuribo>(this);
	//m_pStateMachine->setCurrentState(KuriboStateRun::instance());
}

Kuribo::~Kuribo()
{
	if (m_pStateMachine) {
		delete m_pStateMachine;
		m_pStateMachine = nullptr;
	}
}

void Kuribo::update()
{
	m_turned = false;

	if (m_dead
		|| (g_courseManager.getScroll() > m_position.x + m_size.x + (PART_SIZE * 4))
		|| (g_courseManager.getScroll() + SCREEN_WIDTH + (PART_SIZE * 4) < m_position.x)) {
		m_visible = false;
		return;
	}

	m_counter++;

	m_pStateMachine->update();

	m_visible = true;
}

void Kuribo::draw()
{
	if (!m_visible)
		return;

	g_textureManager.setTexture(m_state == KURIBO_STATE_MOVE ? TEXTURE_KURIBO_RUN : TEXTURE_KURIBO_SQUISH);
	Rect::draw();
	g_textureManager.unbindTexture();

	if (Game::m_debugInfo) {
		Rect::drawWire();

		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);// GLbitfield mask
		glPushAttrib(GL_ALL_ATTRIB_BITS);// GLbitfield mask
		glDisable(GL_TEXTURE_2D);// GLenum cap
		glColor3ub(0xff, 0x00, 0x00);
		glEnableClientState(GL_VERTEX_ARRAY);// GLenum array
		glVertexPointer(
			2,						// GLint size
			GL_FLOAT,				// GLenum type
			0,						// GLsizei stride
			&m_leftPoint);	// const GLvoid * pointer
		glDrawArrays(
			GL_POINTS,				// GLenum mode
			0,						// GLint first
			1);	// GLsizei count

		glVertexPointer(
			2,						// GLint size
			GL_FLOAT,				// GLenum type
			0,						// GLsizei stride
			&m_rightPoint);	// const GLvoid * pointer
		glDrawArrays(
			GL_POINTS,				// GLenum mode
			0,						// GLint first
			1);	// GLsizei count

		glColor3ub(0x00, 0x00, 0xff);
		glVertexPointer(
			2,						// GLint size
			GL_FLOAT,				// GLenum type
			0,						// GLsizei stride
			&m_topPoints);	// const GLvoid * pointer
		glDrawArrays(
			GL_POINTS,					// GLenum mode
			0,							// GLint first
			KURIBO_TOP_POINT_COUNT);	// GLsizei count

		glColor3ub(0x00, 0xff, 0x00);
		glVertexPointer(
			2,						// GLint size
			GL_FLOAT,				// GLenum type
			0,						// GLsizei stride
			m_bottomPoints.data());	// const GLvoid * pointer
		glDrawArrays(
			GL_POINTS,				// GLenum mode
			0,						// GLint first
			static_cast<GLsizei>(m_bottomPoints.size()));	// GLsizei count

		glPopAttrib();
		glPopClientAttrib();

	}
}

void Kuribo::kill()
{
	m_pStateMachine->changeState(KuriboStateDie::instance());
}

void Kuribo::intersectAllKuribo(vector<Kuribo>& kuribos)
{
	vector<Kuribo>::iterator iter = kuribos.begin();
	for (; iter != kuribos.end(); ++iter) {
		if (this->getRect() == iter->getRect() || m_turned)
			continue;

		if (this->intersect(*iter)) {
			turn();
			m_turned = true;
		}
	}
}

Rect Kuribo::getRect() const
{
	return Rect(m_size, m_position);
}
