#include "Koopa.h"
#include "KoopaStates.h"
#include "world/Course.h"
#include "world/Part.h"
#include "common/font.h"
#include "player/Player.h"
#include "TextureManager.h"
#include "animation/Animation.h"

#define KOOPA_MOVE_SPEED	0.2f

using namespace glm;
using namespace std;

Koopa::Koopa()
	: Koopa(0.0f, 0.0f)
{
}

Koopa::Koopa(glm::vec2 _position)
	: Koopa(_position.x, _position.y)
{
}

Koopa::Koopa(float _x, float _y)
	: m_flag(KOOPA_FLAG_COLLISION | KOOPA_FLAG_ANIMATION | KOOPA_FLAG_FALLING)
	, m_jumpCount(0)
	, m_counter(0)
	, m_state(KOOPA_STATE_IDLE)
	, m_speed(0, 0)
	, m_fire()
	, m_actionRange{0.0f, 0.0f}
	, Sprite(vec2(KOOPA_WIDTH, KOOPA_HEIGHT), vec2(_x, _y))
{
	m_pStateMachine = new StateMachine<Koopa>(this);
	m_pStateMachine->setCurrentState(KoopaStateIdle::instance());

	m_animationController.setAnimation(ANIMATION_KOOPA);
}

Koopa::~Koopa()
{
	if (m_pStateMachine) {
		delete m_pStateMachine;
		m_pStateMachine = NULL;
	}
}

void Koopa::reset()
{
	m_flag = KOOPA_FLAG_COLLISION | KOOPA_FLAG_ANIMATION | KOOPA_FLAG_FALLING;
	m_pStateMachine->changeState(KoopaStateIdle::instance());
	m_animationController.setAnimation(ANIMATION_KOOPA);
}

void Koopa::update()
{
	// プレイヤーとの距離がKOOPA_UPDATE_DISTANCEより離れていたら更新しない
	if (distance(g_player.m_position.x, m_position.x) > KOOPA_UPDATE_DISTANCE)
		return;

	// アニメーションを更新
	if (m_flag & KOOPA_FLAG_ANIMATION)
		m_animationController.update();

	m_counter++;

	// プレイヤーの方向にテクスチャを向ける
	m_flip = m_position.x < g_player.m_position.x ? RECT_FLIP_HORIZONTAL : RECT_FLIP_NONE;

	// ステートマシンを更新
	m_pStateMachine->update();

	// ファイヤーを更新
	m_fire.update();

	if ((m_flag & KOOPA_FLAG_FALLING) 
		&& !(m_flag & KOOPA_FLAG_DEAD)) {
		m_speed.y += 0.2f;
	}

	// スピードの影響を与える
	m_position += m_speed;


	// プレイヤーとの当たり判定
	if (this->intersect(g_player) 
		|| m_fire.intersect(g_player)) {
		g_player.kill();
	}

	// 当たり判定
	if (m_flag & KOOPA_FLAG_COLLISION) {
		m_bottomPoints.clear();
		m_bottomPoints.push_back(m_position + vec2(1, m_size.y));
		m_bottomPoints.push_back(m_position + vec2(m_size.x - 1, m_size.y));

		m_flag |= KOOPA_FLAG_FALLING;
		if (m_speed.y >= 0)
			for (std::vector<vec2>::iterator iter = m_bottomPoints.begin();
				iter != m_bottomPoints.end();
				iter++) {
			if (g_courseManager.intersect(*iter)) {
				vec2 bottom = ((ivec2)*iter / PART_SIZE) * PART_SIZE;
				m_position.y = bottom.y - KOOPA_HEIGHT;
				m_flag &= ~KOOPA_FLAG_FALLING;
				break;
			}
		}
	}
}

void Koopa::draw()
{
	if (m_flag & KOOPA_FLAG_DEAD)
		return;

	g_textureManager.setTexture((TEXTURE)g_animations[m_animationController.m_animation].m_keys[m_animationController.m_time]);
	Rect::draw();
	g_textureManager.unbindTexture();

	m_fire.draw();

	if (Game::m_debugInfo) {
		Rect::drawWire();

		fontBegin();
		fontColor(0x00, 0xff, 0xff);
		fontBackgroundColor(true);
		fontDraw("DEAD:%d JUMP:%d FALL:%d COUNT:%d\nCOLLISION:%d ANIMATION:%d",
			m_flag & KOOPA_FLAG_DEAD, (m_flag & KOOPA_FLAG_JUMPING) >> 1,
			(m_flag & KOOPA_FLAG_FALLING) >> 2,
			m_jumpCount,
			(m_flag & KOOPA_FLAG_COLLISION) >> 3,
			(m_flag & KOOPA_FLAG_ANIMATION) >> 4);
		fontBackgroundColor(false);
		fontColor(0xff, 0xff, 0xff);
		fontEnd();
	}
}

void Koopa::kill()
{
	if (g_player.m_clear) {
		m_pStateMachine->changeState(KoopaStateDie::instance());
	}
}

bool Koopa::isDead()
{
	return m_flag & KOOPA_FLAG_DEAD;
}

void Koopa::setActionRange(const RANGE& _range)
{
	m_actionRange = _range;
}

void Koopa::nextMovement()
{
	int move = KOOPA_MOVE_STOP;
	float offset = 32.0f;
	
	if (m_position.x - offset < m_actionRange.start) {
		move = KOOPA_MOVE_BACK;
	} else if (m_position.x + m_size.x + offset > m_actionRange.end) {
		move = KOOPA_MOVE_FRONT;
	} else {
		// ランダムで動きを決める
		move = rand() % KOOPA_MOVE_MAX;
	}

	switch (move) {
	case KOOPA_MOVE_FRONT:
		m_speed.x = -KOOPA_MOVE_SPEED;
		break;
	case KOOPA_MOVE_BACK:
		m_speed.x = KOOPA_MOVE_SPEED;
		break;
	case KOOPA_MOVE_STOP:
		m_speed.x = 0;
		break;
	}
}
