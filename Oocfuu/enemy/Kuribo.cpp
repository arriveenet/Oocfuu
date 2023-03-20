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
	: m_dead(false)
	, m_falling(false)
	, m_state(KURIBO_STATE_MOVE)
	, m_counter(0)
	, m_speed(-KURIBO_SPEED, 0)
	, m_rightPoint(0, 0)
	, m_leftPoint(0, 0)
	, m_topPoints{ vec2(0.0f, 0.0f) }
{
	m_size = { KURIBO_WIDTH, KURIBO_HEIGHT };
	m_position = { _x, _y };
}

void Kuribo::update()
{
	if (
		(m_dead)
		|| (g_courseManager.getScroll() > m_position.x + m_size.x + (PART_SIZE * 4))
		|| (g_courseManager.getScroll() + SCREEN_WIDTH + (PART_SIZE * 4) < m_position.x)
		)
		return;

	m_counter++;

	switch (m_state) {
	case KURIBO_STATE_MOVE:
	{
		if (Game::m_count % 12 == 0)
			m_flip ^= 1;


		if (m_falling)
			m_speed.y += KUROBO_FALL_SPEED;

		m_position += m_speed;

		m_bottomPoints.clear();

		m_topPoints[0] = m_position + vec2(PLAYER_SIZE / 2, 0);
		m_topPoints[1] = m_position + vec2(2, 5);
		m_topPoints[2] = m_position + vec2(14, 5);
		m_rightPoint = vec2(m_position.x + m_size.x, m_position.y + 8);
		m_leftPoint = vec2(m_position.x, m_position.y + 8);
		m_bottomPoints.push_back(m_position + vec2(1, m_size.y));
		m_bottomPoints.push_back(m_position + vec2(m_size.x - 1, m_size.y));

		// Squish
		for (int i = 0; i < KURIBO_TOP_POINT_COUNT; i++) {
			if (g_player.intersect(m_topPoints[i]) && !g_player.m_dead) {
				g_pSound->play(SOUND_SE_SQUISH);
				m_state = KURIBO_STATE_SQUISH;
				g_player.jump();
				g_game.addScore(100);

				vec2 scorePosition = { m_position.x, m_position.y - KURIBO_HEIGHT - 8 };
				EffectScore score(scorePosition, EffectScore::SCORE_100);
				CourseEffectManager* courseEffectMgr = CourseEffectManager::instance();
				courseEffectMgr->addScore(score);
				break;
			}
		}

		// bool player dead
		if (g_player.intersect(m_rightPoint)
			|| g_player.intersect(m_leftPoint)
			) {
			g_player.kill();
		}

		// Kuribo falling dead
		if (m_position.y > SCREEN_HEIGHT)
			m_state = KURIBO_STATE_DEAD;

		if (g_courseManager.intersect(m_rightPoint)) {
			turn();
		}

		if (g_courseManager.intersect(m_leftPoint)) {
			turn();
		}

		m_falling = true;
		if (m_speed.y >= 0)
			for (vector<vec2>::iterator iter = m_bottomPoints.begin();
				iter != m_bottomPoints.end();
				iter++) {
			if (g_courseManager.intersect(*iter)) {
				vec2 bottom = ((ivec2)*iter / PART_SIZE) * PART_SIZE;
				m_position.y = bottom.y - PLAYER_SIZE;
				m_speed.y = 0;
				m_falling = false;
				break;
			}
		}
	}
		break;
	case KURIBO_STATE_SQUISH:
		if (m_counter > 32) {
			m_counter = 0;
			m_state = KURIBO_STATE_DEAD;
		}
		break;
	case KURIBO_STATE_DEAD:
		m_dead = true;
		break;
	}
}

void Kuribo::draw()
{
	if (
		(m_dead)
		|| (g_courseManager.getScroll() > m_position.x + m_size.x + (PART_SIZE * 4))
		|| (g_courseManager.getScroll() + SCREEN_WIDTH + (PART_SIZE * 4) < m_position.x)
		)
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

void Kuribo::turn()
{
	m_speed.x = -m_speed.x;
}

Rect Kuribo::getRect() const
{
	return Rect(m_size, m_position);
}
