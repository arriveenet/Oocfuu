#include "Kuribo.h"

#include "App.h"
#include "world/Part.h"
#include "world/CourseManager.h"
#include "world/CourseEffect.h"
#include "TextureManager.h"
#include "player/Player.h"
#include "sound/Sound.h"

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
	: m_state(KURIBO_STATE_MOVE)
	, m_counter(0)
	, m_texture(TEXTURE_KURIBO_RUN)
	, m_hitRightPoint(0, 0)
	, m_hitLeftPoint(0, 0)
	, m_topPoints{ vec2(0.0f, 0.0f) }
	, Enemy(vec2(KURIBO_WIDTH, KURIBO_HEIGHT), vec2(_x, _y))
{
	m_enemyType = EnemyType::Kuribo;
	m_pStateMachine = new StateMachine<Kuribo>(this);
	m_pStateMachine->setCurrentState(KuriboStateRun::instance());
}

Kuribo::Kuribo(const Kuribo& _kuribo)
	: Kuribo(_kuribo.m_position)
{
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

	g_textureManager.setTexture(m_texture);
	Sprite::draw();
	g_textureManager.unbindTexture();

	if (Game::m_debugInfo) {
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);	// GLbitfield mask
		glPushAttrib(GL_ALL_ATTRIB_BITS);				// GLbitfield mask
		glDisable(GL_TEXTURE_2D);						// GLenum cap
		glColor3ub(0xff, 0x00, 0x00);
		glEnableClientState(GL_VERTEX_ARRAY);			// GLenum array

		glBegin(GL_POINTS);
		glVertex2fv((GLfloat*)& m_hitLeftPoint);
		glVertex2fv((GLfloat*)&m_hitRightPoint);
		glEnd();

		glColor3ub(0x00, 0x00, 0xff);
		glVertexPointer(
			2,						// GLint size
			GL_FLOAT,				// GLenum type
			0,						// GLsizei stride
			&m_topPoints);			// const GLvoid * pointer
		glDrawArrays(
			GL_POINTS,					// GLenum mode
			0,							// GLint first
			KURIBO_TOP_POINT_COUNT);	// GLsizei count

		glPopAttrib();
		glPopClientAttrib();

	}

	Enemy::draw();
}

void Kuribo::kill()
{
	m_pStateMachine->changeState(KuriboStateDie::instance());
}
