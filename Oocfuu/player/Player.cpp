#include "Player.h"
#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"
#include "PlayerStateJump.h"
#include "PlayerStateDie.h"
#include "PlayerStateGoal.h"

#include "App.h"
#include "font.h"
#include "Part.h"
#include "Course.h"
#include "TextureManager.h"
#include "input/Keyboard.h"
#include "input//Mouse.h"
#include "animation/Animation.h"
#include "sound/Sound.h"
#include "world/GimmickPart.h"
#include "sound/Music.h"
#include "score/Komm_susser_Tod.h"

using namespace glm;
using std::vector;

Player g_player;

Player::Player()
	: m_speed(0.0f, 0.0f)
	, m_acceleration(0.24f)
	, m_jumping(false)
	, m_falling(false)
	, m_pStateContext(new PlayerStateContext(this))
	, m_left(PLAYER_START_LEFT)
	, m_dead(false)
	, m_goal(false)
	, m_clear(false)
	, m_visible(true)
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
	m_dead = false;
	m_goal = false;
	m_clear = false;
	m_visible = true;
}

void Player::respawn(float _x, float _y)
{
	m_flip = RECT_FLIP_NONE;
	m_speed = { 0.0f, 0.0f };
	m_jumping = false;
	m_falling = false;
	m_size = { PLAYER_SIZE, PLAYER_SIZE };
	m_position = { _x, _y };
	m_pStateContext->setStete(new PlayerStateIdle);
	m_animeCtr.setAnimation(ANIMATION_PLAYER_IDLE);
	m_dead = false;
	m_goal = false;
	m_clear = false;
	m_visible = true;
}

void Player::update()
{
	m_animeCtr.update();
	m_pStateContext->update();

	// ジャンプ中かつ落下フラグが立っていれば重力の影響を与える
	if (m_falling 
		&& (!m_dead)
		&& (m_pStateContext->getStateEnum() != PLAYER_STATE_JUMP)
		&& (m_pStateContext->getStateEnum() != PLAYER_STATE_GOAL)) {
		m_speed.y += PLAYER_GRAVITY;
	}

	// 速度の影響を座標に与える
	m_position += m_speed;

	// 落下死判定
	if ((m_position.y >= SCREEN_HEIGHT)
		&& (!m_dead)) {
		//g_pSound->play(SOUND_SE_DIE);
		m_speed = { 0.0f, 0.0f };
		m_dead = true;
		m_pStateContext->setStete(new PlayerStateDie);
	}

	// 当たり判定
	m_leftPoints.clear();
	m_rightPoints.clear();
	m_bottomPoints.clear();
	m_topPoints.clear();

	// 左の当たり判定ポイント
	m_leftPoints.push_back(m_position);
	// 右の当たり判定ポイント
	m_rightPoints.push_back(m_position + m_size + vec2(0, -PART_SIZE));
	// 下の当たり判定ポイント
	m_bottomPoints.push_back(m_position + vec2(1, PLAYER_SIZE));
	m_bottomPoints.push_back(m_position + vec2(PLAYER_SIZE - 1, PLAYER_SIZE));
	// 上の当たり判定ポイント
	m_topPoints.push_back(m_position + vec2(PLAYER_SIZE / 2, 0));

	bool topHit = false;
	int parts = PART_NONE;
	if (!m_dead) {
		for (vector<vec2>::iterator iter = m_topPoints.begin();
			iter != m_topPoints.end();
			iter++) {
			if (g_courseManager.intersect(*iter, &parts)) {
				if (parts == PART_GOAL_POLE)
					continue;
				vec2 top = (ivec2)*iter / PART_SIZE * PART_SIZE;
				m_position.y = top.y + PLAYER_SIZE;
				m_speed.y = 0;
				m_jumping = false;
				//m_falling = true;
				topHit = true;
				break;
<<<<<<< HEAD
			}
		}

		if (!topHit && !m_goal) {
			for (vector<vec2>::iterator iter = m_rightPoints.begin();
				iter != m_rightPoints.end();
				iter++) {
				int parts = PART_NONE;
				if (g_courseManager.intersect(*iter, &parts)) {
					vec2 right = (ivec2)*iter / PART_SIZE * PART_SIZE;
					m_position.x = right.x - PLAYER_SIZE;
					m_speed.x = 0;
					m_falling = true;
					// プレイヤーがゴール
					if ((parts == PART_GOAL_POLE) && (!m_goal)) {
						//printf("Player is goal\n");
						m_goal = true;
						g_game.stopTimer();
						m_pStateContext->setStete(new PlayerStateGoal);
						break;
					} else if (parts == PART_AXE && (!m_clear)) {
						m_clear = true;
						printf("clear\n");
						g_game.stopTimer();
						g_music.resetScore();
						g_music.setScore(AUDIO_CHANNEL_PULSE0, komm::pulse0, komm::PULSE0_COUNT);
						g_music.setScore(AUDIO_CHANNEL_PULSE1, komm::triangle, komm::TRIANGLE_COUNT);
						g_music.play();
					}
					break;
				}
			}

		for (vector<vec2>::iterator iter = m_leftPoints.begin();
			iter != m_leftPoints.end();
			iter++) {
=======
			}
		}

		if (!topHit && !m_goal) {
			for (vector<vec2>::iterator iter = m_rightPoints.begin();
				iter != m_rightPoints.end();
				iter++) {
				int parts = PART_NONE;
				if (g_courseManager.intersect(*iter, &parts)) {
					vec2 right = (ivec2)*iter / PART_SIZE * PART_SIZE;
					m_position.x = right.x - PLAYER_SIZE;
					m_speed.x = 0;
					m_falling = true;
					// プレイヤーがゴール
					if ((parts == PART_GOAL_POLE) && (!m_goal)) {
						//printf("Player is goal\n");
						m_goal = true;
						g_game.stopTimer();
						m_pStateContext->setStete(new PlayerStateGoal);
						break;
					} else if (parts == PART_AXE && (!m_clear)) {
						m_clear = true;
						printf("clear\n");
						g_game.stopTimer();
						g_music.resetScore();
						g_music.setScore(AUDIO_CHANNEL_PULSE0, komm::pulse0, komm::PULSE0_COUNT);
						g_music.setScore(AUDIO_CHANNEL_PULSE1, komm::triangle, komm::TRIANGLE_COUNT);
						g_music.play();
					}
					break;
				}
			}

			for (vector<vec2>::iterator iter = m_leftPoints.begin();
				iter != m_leftPoints.end();
				iter++) {
				if (g_courseManager.intersect(*iter)) {
					vec2 left = (ivec2)*iter / PART_SIZE * PART_SIZE;
					m_position.x = left.x + PLAYER_SIZE;
					m_speed.x = 0;
					break;
				}
			}
		}

		// 地面との当たり判定
		m_falling = true;
		vec2 liftPosition;
		vec2 liftSpeed;
		if (m_speed.y >= 0)
			for (vector<vec2>::iterator iter = m_bottomPoints.begin();
				iter != m_bottomPoints.end();
				iter++) {
>>>>>>> master
			if (g_courseManager.intersect(*iter)) {
				vec2 bottom = ((ivec2)*iter / PART_SIZE) * PART_SIZE;
				m_position.y = bottom.y - PLAYER_SIZE;
				m_speed.y = 0;
				m_jumping = false;
				m_falling = false;
				break;
			} else if (g_gmmickPart.intersectLift(*iter, liftPosition, liftSpeed)) {	// リフトの当たり判定
				m_position.y = liftPosition.y - PLAYER_SIZE;
				//m_speed = liftSpeed;
				m_jumping = false;
				m_falling = false;
				break;
			}
		}
	}
}

void Player::draw()
{
	if (m_visible) {
		g_textureManager.setTexture((TEXTURE)g_animations[m_animeCtr.m_animation].m_keys[m_animeCtr.m_time]);
		Rect::draw();
		g_textureManager.unbindTexture();
	}

	if (Game::m_debugInfo) {
		fontColor(0x00, 0xff, 0x00);
		fontBackgroundColor(true);
		fontBegin();
		fontPosition(0, 0);
		fontPosition(0, 8 * 4);
		fontDraw("POSITION:%f,%f\n", g_player.m_position.x, g_player.m_position.y);
		fontDraw("SPEED   :%f,%f\n", g_player.m_speed.x, g_player.m_speed.y);
		fontDraw("STATE   :%s\n", m_pStateContext->getString().c_str());
		fontDraw("ANIMATION:%d\n", m_animeCtr.m_animation);
		fontDraw("JUMPING :%d\n", g_player.m_jumping);
		fontDraw("FALLING :%d\n", g_player.m_falling);
		fontDraw("DEAD    :%d\n", g_player.m_dead);
		fontEnd();
		fontBackgroundColor(false);
		fontColor(0xff, 0xff, 0xff);

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

		Rect::drawWire();
	}
}

void Player::kill()
{
	//printf("Player::kill()\n");
	m_dead = true;
	m_pStateContext->setStete(new PlayerStateDie);
}

void Player::jump()
{
<<<<<<< HEAD
=======
	if (m_dead)
		return;

>>>>>>> master
	m_pStateContext->setStete(new PlayerStateJump);
}
