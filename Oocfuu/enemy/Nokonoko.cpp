#include "Nokonoko.h"

#include "Game.h"
#include "TextureManager.h"
#include "world/Part.h"
#include "world/Course.h"

using namespace glm;
using std::vector;

const vec2 NOKONOKO_SIZE = vec2(16, 24);

Nokonoko::Nokonoko()
	: Nokonoko(0.0f, 0.0f)
{
}

Nokonoko::Nokonoko(glm::vec2 _position)
	: Nokonoko(_position.x, _position.y)
{
}

Nokonoko::Nokonoko(float _x, float _y)
	: m_dead(false)
	, m_falling(false)
	, m_state(NOKONOKO_STATE_MOVE)
	, m_counter(0)
	, m_speed(0, 0)
	,Sprite(vec2(NOKONOKO_SIZE), vec2(_x, _y))
{
}

void Nokonoko::update()
{
	switch (m_state) {
	case NOKONOKO_STATE_MOVE:
		m_collision.clear();

		break;
	case NOKONOKO_STATE_SQUISH:
		break;
	case NOKONOKO_STATE_SPIN:
		break;
	case NOKONOKO_STATE_RETURN:
		break;
	case NOKONOKO_STATE_DEAD:
		break;
	case NOKONOKO_STATE_MAX:
		break;
	default:
		break;
	}
}

void Nokonoko::draw()
{
	g_textureManager.setTexture(TEXTURE_NOKONOKO_RUN_1);
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
			&m_position);	// const GLvoid * pointer
		glDrawArrays(
			GL_POINTS,				// GLenum mode
			0,						// GLint first
			1);	// GLsizei count

		glPopAttrib();
		glPopClientAttrib();
	}
}
