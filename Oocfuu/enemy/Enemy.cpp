#include "Enemy.h"
#include "Game.h"
#include "EnemyManager.h"
#include "world/CourseManager.h"

using namespace glm;
using std::vector;

/**
* コンストラクタ
* @param[in] なし
*/
Enemy::Enemy()
	: Enemy(0.0f, 0.0f)
{
}

/**
* コンストラクタ
* @param [in] _position	座標
*/
Enemy::Enemy(const glm::vec2& _position)
	: Enemy(_position.x, _position.y)
{
}

/**
* コンストラクタ
* @param [in] _x	X座標
* @param [in] _y	Y座標
*/
Enemy::Enemy(float _x, float _y)
	: Enemy(glm::vec2(), glm::vec2(_x, _y))
{
}

/**
* コンストラクタ
* @param [in] _size		サイズ
* @param [in] _position	位置
*/
Enemy::Enemy(const glm::vec2& _size, const glm::vec2& _position)
	: m_dead(false)
	, m_visible(false)
	, m_falling(true)
	, m_speed(-ENEMY_SPEED, 0)
	, m_leftPoint()
	, m_rightPoint()
	, m_bottomPoints{}
	, m_animationController()
	, Rect(_size, _position)
{
}

/**
* 敵キャラクターを更新
* @param[in] なし
*/
void Enemy::update()
{
	// 地面についていなければ落下する
	if (m_falling)
		m_speed.y += ENEMY_FALL_SPEED;

	// 当たり判定ポイントを設定する
	m_leftPoint = m_position + vec2(0, m_size.y - PART_SIZE);
	m_rightPoint = m_position + vec2(m_size.x, m_size.y - PART_SIZE);
	m_bottomPoints[0] = m_position + m_size;			// 右下
	m_bottomPoints[1] = m_position + vec2(0, m_size.y);	// 左下

	// コースと左のポイントとの当たり判定
	if (g_courseManager.intersect(m_leftPoint)) {
		vec2 left = (ivec2)m_rightPoint / PART_SIZE * PART_SIZE;
		m_position.x = left.x;
		turnRight();
	}

	// コースと右のポイントとの当たり判定
	if (g_courseManager.intersect(m_rightPoint)) {
		vec2 right = (ivec2)m_leftPoint / PART_SIZE * PART_SIZE;
		m_position.x = right.x;
		turnLeft();
	}

	// 地面との当たり判定
	m_falling = true;
	for (int i = 0; i < ENEMY_BOTTOM_COUNT; i++) {
		if (g_courseManager.intersect(m_bottomPoints[i])) {
			vec2 bottom = ((ivec2)m_bottomPoints[i] / PART_SIZE) * PART_SIZE;
			m_position.y = bottom.y - m_size.y;
			m_speed.y = 0;
			m_falling = false;
			break;
		}
	}
}

/**
* 敵キャラクターを描画
* @param[in] なし
*/
void Enemy::draw()
{
	// デバッグ時のポイントを描画する
	if (Game::m_debugInfo) {
		// 左右のポイントを描画
		glColor3ub(0xff, 0xff, 0x00);
		glBegin(GL_POINTS);
		glVertex2fv((const GLfloat*)&m_leftPoint);
		glVertex2fv((const GLfloat*)&m_rightPoint);
		glEnd();

		// 下のポイントを描画
		glColor3ub(0x00, 0xff, 0x00);
		glBegin(GL_POINTS);
		glVertex2fv((const GLfloat*)&m_bottomPoints[0]);
		glVertex2fv((const GLfloat*)&m_bottomPoints[1]);
		glEnd();
		glColor3ub(0xff, 0xff, 0xff);

		// ワイヤーフレームを描画
		Rect::drawWire();
	}
}

/**
* 敵キャラクター同士の当たり判定
* @param[in] なし
*/
void Enemy::intersectEnemy()
{
	// 画面内のすべての敵キャラクターを取得
	vector<Enemy*> enemies = g_enemyManager.getAllEnemy();
	for (vector<Enemy*>::iterator enemy = enemies.begin();
		enemy != enemies.end();
		++enemy) {
		//　自分自身の場合コンティニュー
		if (this->operator==(*(*enemy))) {
			continue;
		}

		// 左の当たり判定
		if ((*enemy)->intersect(m_leftPoint)) {
			m_position.x = (*enemy)->m_position.x + (*enemy)->m_size.x;
			turnRight();
		}

		// 右の当たり判定
		if ((*enemy)->intersect(m_rightPoint)) {
			m_position.x = (*enemy)->m_position.x - m_size.x;
			turnLeft();
		}
	}
}