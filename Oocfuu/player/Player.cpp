#include "Player.h"
#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"
#include "../TextureManager.h"
#include "../font.h"
#include "../animation/Animation.h"
#include "../Keyboard.h"
#include "../Mouse.h"
#include "../Part.h"
#include "../Course.h"

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

using namespace glm;
using std::vector;

Player g_player;

Player::Player()
	: m_speed(1.0f, 1.0f)
	, m_jumping(false)
	, m_falling(false)
	, m_pStateContext(new PlayerStateContext)
	, m_left(PLAYER_START_LEFT)
{
	m_size = { PLAYER_SIZE, PLAYER_SIZE };
	m_position = { PLAYER_DEFAULT_X, PLAYER_DEFAULT_Y };
	m_pStateContext->setStete(new PlayerStateIdle);
	m_animeCtr.setAnimation(ANIMATION_PLAYER_IDLE);
}

Player::~Player()
{
	if (m_pStateContext) {
		delete m_pStateContext;
		m_pStateContext = NULL;
	}
}

void Player::init()
{

}

void Player::reset()
{
	m_flip = RECT_FLIP_NONE;
	m_speed = { 0.0f, 0.0f };
	m_jumping = false;
	m_falling = false;
	m_size = { PLAYER_SIZE, PLAYER_SIZE };
	m_position = { PLAYER_DEFAULT_X, PLAYER_DEFAULT_Y };
	m_pStateContext->setStete(new PlayerStateIdle);
	m_animeCtr.setAnimation(ANIMATION_PLAYER_IDLE);
	m_left = PLAYER_START_LEFT;
}

void Player::update()
{
	m_animeCtr.update();
	m_pStateContext->update(this);

	// 当たり判定
	m_leftPoints.clear();
	m_rightPoints.clear();
	m_bottomPoints.clear();
	m_topPoints.clear();

	m_leftPoints.push_back(m_position);
	m_rightPoints.push_back(m_position + m_size + vec2(0, -PART_SIZE));

	m_bottomPoints.push_back(m_position + vec2(1, PLAYER_SIZE));
	m_bottomPoints.push_back(m_position + vec2(PLAYER_SIZE - 1, PLAYER_SIZE));

	m_topPoints.push_back(m_position + vec2(PLAYER_SIZE / 2, -1));

	bool topHit = false;
	static int parts;
	for (vector<vec2>::iterator iter = m_topPoints.begin();
		iter != m_topPoints.end();
		iter++) {
		if (g_course.intersect(*iter, &parts)) {
			m_jumping = false;
			topHit = true;
			break;
		}
	}

	if (!topHit) {
		for (vector<vec2>::iterator iter = m_rightPoints.begin();
			iter != m_rightPoints.end();
			iter++) {
			if (g_course.intersect(*iter)) {
				vec2 right = (ivec2)*iter / PART_SIZE * PART_SIZE;
				m_position.x = right.x - PLAYER_SIZE;
				m_speed.x = 0;
				m_falling = true;
				break;
			}
		}

		for (vector<vec2>::iterator iter = m_leftPoints.begin();
			iter != m_leftPoints.end();
			iter++) {
			if (g_course.intersect(*iter)) {
				vec2 left = (ivec2)*iter / PART_SIZE * PART_SIZE;
				m_position.x = left.x + PLAYER_SIZE;
				m_speed.x = 0;
				break;
			}
		}
	}
	// 地面との当たり判定

	if (m_speed.y >= 0)
		for (vector<vec2>::iterator iter = m_bottomPoints.begin();
			iter != m_bottomPoints.end();
			iter++) {
		if (g_course.intersect(*iter)) {
			vec2 bottom = ((ivec2)*iter / PART_SIZE) * PART_SIZE;
			m_position.y = bottom.y - PLAYER_SIZE;
			m_jumping = false;
			m_falling = false;
			break;
		}
	}
}

void Player::draw()
{
	g_textureManager.setTexture((TEXTURE)g_animations[m_animeCtr.m_animation].m_keys[m_animeCtr.m_time]);
	Rect::draw();
	g_textureManager.unbindTexture();

	glColor3ub(0x00, 0xff, 0x00);
	fontBegin();
	fontPosition(0, 0);
	fontPosition(0, 8 * 4);
	fontDraw("POSITION:%f,%f\n", g_player.m_position.x, g_player.m_position.y);
	fontDraw("SPEED   :%f,%f\n", g_player.m_speed.x, g_player.m_speed.y);
	fontDraw("STATE   :%s\n", m_pStateContext->getString().c_str());
	fontDraw("ANIMATION:%d\n", m_animeCtr.m_animation);
	fontDraw("JUMPING :%d\n", g_player.m_jumping);
	fontDraw("FALLING :%d\n", g_player.m_falling);
	fontEnd();
	glColor3ub(0xff, 0xff, 0xff);

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

		glColor3ub(0xff, 0x00, 0xff);
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
