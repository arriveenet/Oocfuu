#include "Koopa.h"
#include "KoopaStates.h"
#include "world/CourseManager.h"
#include "world/Part.h"
#include "common/font.h"
#include "player/Player.h"
#include "TextureManager.h"
#include "animation/Animation.h"

#define KOOPA_MOVE_SPEED	0.2f

using namespace glm;
using namespace std;

static const char* getStateString(KOOPA_STATE _state)
{
	const char* str;
	switch (_state) {
	case KOOPA_STATE_IDLE:
		str = "KOOPA_STATE_IDLE";
		break;
	case KOOPA_STATE_JUMP:
		str = "KOOPA_STATE_JUMP";
		break;
	case KOOPA_STATE_FIRE:
		str = "KOOPA_STATE_FIRE";
		break;
	case KOOPA_STATE_DIE:
		str = "KOOPA_STATE_DIE";
		break;
	case KOOPA_STATE_MAX:
		str = "KOOPA_STATE_MAX";
		break;
	default:
		str = "UNKNOWN";
		break;
	}

	return str;
}

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
	, m_fires()
	, m_actionRange{0.0f, 0.0f}
	, Enemy(vec2(KOOPA_WIDTH, KOOPA_HEIGHT), vec2(_x, _y))
{
	m_enemyType = EnemyType::Koopa;

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
	//printf("distance: %f\n", distance(g_player.m_position.x, m_position.x));
	// プレイヤーとの距離がKOOPA_UPDATE_DISTANCEより離れていたら更新しない
	if (distance(g_player.getPosition().x, m_position.x) > KOOPA_UPDATE_DISTANCE)
		return;

	// アニメーションを更新
	if (m_flag & KOOPA_FLAG_ANIMATION)
		m_animationController.update();

	m_counter++;

	// プレイヤーの方向にテクスチャを向ける
	setFlip(m_position.x < g_player.getPosition().x ? RECT_FLIP_HORIZONTAL : RECT_FLIP_NONE);

	// ステートマシンを更新
	m_pStateMachine->update();

	// ファイヤーを更新
	for (int i = 0; i < KOOPA_FIRE_MAX; i++) {
		m_fires[i].update();
		if (m_fires[i].intersect(g_player.getRect()))
			g_player.kill();
	}

	if ((m_flag & KOOPA_FLAG_FALLING) 
		&& !m_dead) {
		m_speed.y += 0.2f;
	}

	// スピードの影響を与える
	m_position += m_speed;


	// プレイヤーとの当たり判定
	if (this->intersect(g_player.getRect())) {
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
				++iter) {
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
	if (m_dead)
		return;

	g_textureManager.setTexture((TEXTURE)g_animations[m_animationController.m_animation].m_keys[m_animationController.m_time]);
	Sprite::draw();
	g_textureManager.unbindTexture();

	for (int i = 0; i < KOOPA_FIRE_MAX; i++) {
		m_fires[i].draw();
	}

	if (Game::m_debugInfo) {
		Sprite::drawWire();

		fontBegin();
		fontColor(0x00, 0xff, 0xff);
		fontBackgroundColor(true);
		fontDraw("DEAD:%d JUMP:%d FALL:%d COUNT:%d\nCOLLISION:%d ANIMATION:%d\n",
			m_dead, (m_flag & KOOPA_FLAG_JUMPING) >> 1,
			(m_flag & KOOPA_FLAG_FALLING) >> 2,
			m_jumpCount,
			(m_flag & KOOPA_FLAG_COLLISION) >> 3,
			(m_flag & KOOPA_FLAG_ANIMATION) >> 4);
		fontDraw("STATE:%s\n", getStateString(m_state));
		fontBackgroundColor(false);
		fontColor(0xff, 0xff, 0xff);
		fontEnd();
	}
}

void Koopa::fire()
{
	for (int i = 0; i < KOOPA_FIRE_MAX; i++) {
		if (m_fires[i].isEnable())
			continue;

		// 向いている方向にファイヤーを向ける
		float x = isFlipX() ? m_position.x + m_size.x : m_position.x - 24.0f;
		// 高さをランダムで決める(高:10%、中:10%、低:80%)
		int r = rand() % 100;
		FIRE_HEIGHT hight;
		if (r < 10) {
			hight = FIRE_HEIGHT_HIGH;
		} else if(r < 20){
			hight = FIRE_HEIGHT_MIDDLE;
		} else {
			hight = FIRE_HEIGHT_LOW;
		}
		int direction = isFlipX() ? RECT_FLIP_HORIZONTAL : RECT_FLIP_NONE;
		m_fires[i].start(vec2(x, m_position.y), hight, direction);
		return;
	}
}

void Koopa::kill()
{
	if (g_player.m_clear) {
		m_pStateMachine->changeState(KoopaStateDie::instance());
	}
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
