#pragma once
#include "Rect.h"
#include "common/Entity.h"
#include "animation/AnimationController.h"
#include <glm/glm.hpp>
#include <vector>

#define ENEMY_SPEED			0.5f
#define ENEMY_FALL_SPEED	0.3f
#define ENEMY_BOTTOM_COUNT	2

enum ENEMYTYPE {
	ENEMYTYPE_KURIBO,
	ENEMYTYPE_NOKONIKO,
	ENEMYTYPE_KOOPA,
	ENEMYTYPE_MAX
};

typedef struct tagENEMY {
	unsigned char id;	// 敵の種類
	unsigned short x;	// 敵のX座標
	unsigned short y;	// 敵のY座標
} ENEMY;

// 敵の抽象クラス
class Enemy : public Rect, Entity{
public:
	// コンストラクタ
	Enemy();
	explicit Enemy(const glm::vec2& _position);
	explicit Enemy(float x, float y);
	explicit Enemy(const glm::vec2& _size, const glm::vec2& _position);

	// デストラクタ
	virtual ~Enemy() {};

	// 敵キャラクターを更新
	virtual void update() override;

	// 敵キャラクターを描画
	virtual void draw() override;

	// 左右のスピードを反転する
	virtual void turn();

	// 敵同士の衝突処理
	void intersectEnemy();

	// 敵キャラクターをキルする
	virtual void kill();

	// 敵キャラクターが死んでいるか
	bool isDead();

	// 敵キャラクターが生きているか
	bool isAlive();

	// 敵キャラクターを描画するか
	bool isVisble();

protected:
	// 左に向く
	void turnLeft();

	// 右に向く
	void turnRight();

protected:
	bool		m_dead;				// 死亡フラグ
	bool		m_visible;			// 描画フラグ
	bool		m_falling;			// 落下フラグ
	glm::vec2	m_speed;			// スピード
	glm::vec2	m_leftPoint;		// 左のポイント
	glm::vec2	m_rightPoint;		// 右のポイント
	glm::vec2	m_bottomPoints[ENEMY_BOTTOM_COUNT];	// 下のポイント
	AnimationController m_animationController;		// アニメーションコントローラー
};

/**
* X軸のスピードを反転する
*/
inline void Enemy::turn()
{
	m_speed.x = -m_speed.x;
}

/**
* 敵キャラクターをキルする
*/
inline void Enemy::kill()
{
	m_dead = true;
}

/**
* 敵キャラクターが死んでいるか
*/
inline bool Enemy::isDead()
{
	return m_dead;
}

/**
* 敵キャラクターが生きているか
*/
inline bool Enemy::isAlive()
{
	return !m_dead;
}

/**
* 敵キャラクターを描画するか
*/
inline bool Enemy::isVisble()
{
	return m_visible;
}

/**
* 左に向く
*/
inline void Enemy::turnLeft()
{
	m_flip = RECT_FLIP_NONE;
	m_speed.x = -ENEMY_SPEED;
}

/**
* 右に向く
*/
inline void Enemy::turnRight()
{
	m_flip = RECT_FLIP_HORIZONTAL;
	m_speed.x = ENEMY_SPEED;
}
