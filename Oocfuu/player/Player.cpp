#include "Player.h"
#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"
#include "PlayerStateJump.h"
#include "PlayerStateDie.h"
#include "PlayerStateGoal.h"
#include "PlayerStateClear.h"

#include "App.h"
#include "TextureManager.h"
#include "common/font.h"
#include "enemy/EnemyManager.h"
#include "world/Part.h"
#include "world/Course.h"
#include "world/GimmickPart.h"
#include "input/Keyboard.h"
#include "input//Mouse.h"
#include "animation/Animation.h"
#include "sound/Sound.h"
#include "sound/Music.h"

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
	m_pStateContext->setStete(new PlayerStateIdle);
	m_animationController.setAnimation(ANIMATION_PLAYER_IDLE);
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
	m_position = { 0, 0 };
	m_pStateContext->setStete(new PlayerStateIdle);
	m_animationController.setAnimation(ANIMATION_PLAYER_IDLE);
	m_left = PLAYER_START_LEFT;
	m_dead = false;
	m_goal = false;
	m_clear = false;
	m_visible = true;
	m_messageController.reset();
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
	m_animationController.setAnimation(ANIMATION_PLAYER_IDLE);
	m_dead = false;
	m_goal = false;
	m_clear = false;
	m_visible = true;
}

void Player::update()
{
	m_animationController.update();
	m_pStateContext->update();
	m_messageController.update();

	// ジャンプ中かつ落下フラグが立っていれば重力の影響を与える
	if (m_falling 
		&& (!m_dead)
		&& (m_pStateContext->getStateEnum() != PLAYER_STATE_JUMP)
		&& (m_pStateContext->getStateEnum() != PLAYER_STATE_GOAL)
		&& (m_pStateContext->getStateEnum() != PLAYER_STATE_CLEAR)) {
		m_speed.y += PLAYER_GRAVITY;
	}

	// 速度の影響を座標に与える
	m_position += m_speed;

	// 落下死判定
	if ((m_position.y >= SCREEN_HEIGHT)
		&& (!m_dead)) {
		m_speed = { 0.0f, 0.0f };
		m_dead = true;
		m_pStateContext->setStete(new PlayerStateDie);
	}

	// コインとの当たり判定
	g_courseManager.intersectCoin(this);

	// ワールドクリア判定
	Rect aex;
	if (g_courseManager.getClearAex(aex) && !m_clear) {
		if (this->intersect(aex)) {
			m_clear = true;
			g_game.m_timer.stop();
			m_pStateContext->setStete(new PlayerStateClear);
			g_enemyManager.koopaKill();

			// 斧を消す
			g_courseManager.setParts(
				vec2(aex.m_position.x / PART_SIZE, aex.m_position.y / PART_SIZE),
				PART_NONE);
		}
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

	if (!m_dead) {
		bool topHit = false;
		for (vector<vec2>::iterator iter = m_topPoints.begin();
			iter != m_topPoints.end();
			iter++) {
			if (g_courseManager.intersect(*iter)) {
				int parts = g_courseManager.getParts(*iter);
				if (parts == PART_GOAL_POLE)
					continue;
				g_courseManager.hitBlock(*iter);
				vec2 top = (ivec2)*iter / PART_SIZE * PART_SIZE;
				m_position.y = top.y + PLAYER_SIZE;
				m_speed.y = 0;
				m_jumping = false;
				//m_falling = true;
				topHit = true;
				break;
			}
		}

		if (!topHit && !m_goal) {
			for (vector<vec2>::iterator iter = m_rightPoints.begin();
				iter != m_rightPoints.end();
				iter++) {

				// プレイヤーがゴール
				if ((g_courseManager.getParts(*iter) == PART_GOAL_POLE) && (!m_goal)) {
					//printf("Player is goal\n");
					m_goal = true;
					g_game.m_timer.stop();
					m_pStateContext->setStete(new PlayerStateGoal);
					break;
				}

				if (g_courseManager.intersect(*iter)) {
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
			if (g_courseManager.intersect(*iter)) {
				vec2 bottom = ((ivec2)*iter / PART_SIZE) * PART_SIZE;
				m_position.y = bottom.y - PLAYER_SIZE;
				m_speed.y = 0;
				m_jumping = false;
				m_falling = false;
				break;
			} // リフトの当たり判定
			else if (g_gmmickPart.intersectLift(*iter, liftPosition, liftSpeed)) {	
				m_position.y = liftPosition.y - PLAYER_SIZE;
				m_position.x += liftSpeed.x;
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
		g_textureManager.setTexture((TEXTURE)g_animations[m_animationController.m_animation].m_keys[m_animationController.m_time]);
		Rect::draw();
		g_textureManager.unbindTexture();
	}

	if (Game::m_debugInfo) {
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

	m_messageController.draw();
}

void Player::kill()
{
	if (m_dead)
		return;

	m_dead = true;
	m_pStateContext->setStete(new PlayerStateDie);
}

void Player::jump()
{
	if (m_dead)
		return;

	m_pStateContext->setStete(new PlayerStateJump);
}

void Player::oneUp()
{
	m_left++;
	g_pSound->play(SOUND_SE_1UP);
}
