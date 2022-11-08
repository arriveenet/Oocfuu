#include "Player.h"
#include "App.h"
#include "Keyboard.h"
#include "Part.h"
#include "Course.h"
#include "TextureManager.h"
#include "sound/Sound.h"
#include "animation/Animation.h"

#include <al.h>

using namespace std;

Player g_player;

Player::Player()
	: m_animationController()
	, m_speed(0,0)
	, m_jumpCount(0)
	, m_jumping(false)
	, m_falling(false)
	, m_left(3)
	, m_counter(0)
{}

int Player::init()
{
	m_size = vec2(16,16);
	m_position = vec2(PLAYER_DEFAULT_X, PLAYER_DEFAULT_Y);
	g_course.m_scroll = 0.f;
	m_animationController.setAnimation(ANIMATION_PLAYER_IDLE);

	return 0;
}

void Player::reset()
{
	m_size = vec2(16, 16);
	m_speed = vec2(0, 0);
	m_position = vec2(16 * 2.5, 16 * 12);
	g_course.m_scroll = 0.f;
	m_animationController.setAnimation(ANIMATION_PLAYER_IDLE);
}

void Player::update()
{
	AnimationController& ac = m_animationController;
	ac.update();	// Animation update

	// Keyborad input
	static float acceleration = 0.2f;
	if (Keyboard::m_pressed[PLAYER_KEY_RIGHT]) {
		m_speed.x += acceleration;
	}

	if (Keyboard::m_pressed[PLAYER_KEY_LEFT]) {
		m_speed.x -= acceleration;
	}

	if (Keyboard::m_nowPressed[PLAYER_KEY_JUMP]
		&& (!m_falling)) {
		int r = rand() % 2;
		g_pSound->play(r);

		m_jumping = m_falling = true;
		m_jumpCount = 0;
		ac.setAnimation(ANIMATION_PLAYER_JUMP);
	}

	if (m_falling) {
		if (m_jumping) {
			m_speed.y = -4.5f;
			if (
				(++m_jumpCount >= PLAYER_JUMP_COUNT_MAX)
				|| (!Keyboard::m_pressed[PLAYER_KEY_JUMP])
				)
				m_jumping = false;
		}
		m_speed.y += .4f;
	}

	{
		static int one = 0x100;
		ivec2 fixed = m_speed * (float)one;
		fixed = fixed * 0xe0 / one;
		m_speed = (vec2)fixed / (float)one;
	}
	m_position += m_speed;

	// Player fall
	if (m_position.y > SCREEN_HEIGHT) {
		m_speed = vec2(0, 0);
		m_position = vec2(PLAYER_DEFAULT_X, PLAYER_DEFAULT_Y);
		g_course.m_scroll = 0;
	}

	// Player is goal
	if (m_position.x > 3160.f)
		//printf("Player is goal\n");

	// Don't go off screen
	{
		float left = g_course.m_scroll - PLAYER_SIZE * 3 / 8;
		if (m_position.x < left) {
			m_position.x = left;
			m_speed.x = 0;
		}
	}

	{
		float right = (float)g_course.getWidth() * PART_SIZE - PLAYER_SIZE * 5 / 8;
		if (m_position.x > right) {
			m_position.x = right;
			m_speed.x = 0;
		}
	}

	if (
		(m_speed.x > 0)
		&& (m_position.x > g_course.m_scroll + SCREEN_WIDTH / 2 - PLAYER_SIZE / 2)
		)
		g_course.m_scroll += m_speed.x;

	g_course.m_scroll = std::min(g_course.m_scroll, (float)(g_course.getWidth() * PART_SIZE - SCREEN_WIDTH));

	// Animation settings
	switch (ac.m_animation) {
	case ANIMATION_PLAYER_IDLE:
		if (Keyboard::m_pressed[PLAYER_KEY_LEFT] || Keyboard::m_pressed[PLAYER_KEY_RIGHT])
			ac.setAnimation(ANIMATION_PLAYER_RUN);

		if (Keyboard::m_pressed[PLAYER_KEY_LEFT])
			m_flip = RECT_FLIP_HORIZONTAL;

		if (Keyboard::m_pressed[PLAYER_KEY_RIGHT])
			m_flip = RECT_FLIP_NONE;

		break;
	case ANIMATION_PLAYER_RUN:
		if (m_speed.x < 0)
			m_flip = RECT_FLIP_HORIZONTAL;
		if (m_speed.x > 0)
			m_flip = RECT_FLIP_NONE;

		if (length(m_speed) <= 0)
			ac.setAnimation(ANIMATION_PLAYER_IDLE);
		break;
	case ANIMATION_PLAYER_JUMP:
		if (!m_falling) 
			ac.setAnimation(ANIMATION_PLAYER_IDLE);
		break;
	}

	// collision detection
	m_leftPoints.clear();
	m_rightPoints.clear();
	m_bottomPoints.clear();
	m_topPoints.clear();

	m_leftPoints.push_back(m_position);
	m_rightPoints.push_back(m_position + m_size + vec2(0, -PART_SIZE));

	m_bottomPoints.push_back(m_position + vec2(1, PLAYER_SIZE));
	m_bottomPoints.push_back(m_position + vec2(PLAYER_SIZE-1, PLAYER_SIZE));

	m_topPoints.push_back(m_position + vec2(PLAYER_SIZE / 2, -1));

	bool topHit = false;
	static int parts;
	for (vector<vec2>::iterator iter = m_topPoints.begin();
		iter != m_topPoints.end();
		iter++) {
		if (g_course.intersect(*iter, &parts)) {
			vec2 top = (ivec2)*iter / PART_SIZE * PART_SIZE;
			m_position.y = top.y + PLAYER_SIZE;
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

	m_falling = true;
	if(m_speed.y >= 0)
		for (vector<vec2>::iterator iter = m_bottomPoints.begin();
			iter != m_bottomPoints.end();
			iter++) {
		if (g_course.intersect(*iter)) {
			vec2 bottom = ((ivec2)*iter / PART_SIZE) * PART_SIZE;
			m_position.y = bottom.y - PLAYER_SIZE;
			m_speed.y = 0;
			m_falling = false;
			break;
		}
	}
}

void Player::draw()
{
	AnimationController& ac = m_animationController;
	// Texture texture = g_textures[g_animations[ac.m_animation].m_keys[ac.m_time]];
	g_textureManager.setTexture((TEXTURE)g_animations[ac.m_animation].m_keys[ac.m_time]);
	//glBindTexture(
	//GL_TEXTURE_2D,
	//texture.m_texture);
	Rect::draw();
	
	/*
	{
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);// GLbitfield mask
		glPushAttrib(GL_ALL_ATTRIB_BITS);// GLbitfield mask
		glDisable(GL_TEXTURE_2D);// GLenum cap
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
	*/
}

void Player::draw(vec2 _position)
{
	g_textureManager.setTexture(TEXTURE_PLAYER_IDLE);
	Rect rect(m_size, _position);
	rect.draw();
}