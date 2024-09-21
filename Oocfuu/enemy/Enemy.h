#pragma once
#include "Rect.h"
#include "animation/AnimationController.h"
#include <glm/glm.hpp>
#include <vector>

#define ENEMY_SPEED			0.5f
#define ENEMY_FALL_SPEED	0.3f
#define ENEMY_BOTTOM_COUNT	2

/**
 * @brief 敵キャラクターの種類
 */
enum class EnemyType {
	Unknown,	//!< 不明
	Kuribo,		//!< クリボー
	Nokonoko,	//!< ノコノコ
	Koopa,		//!< クッパ
	Max			//!< 敵キャラクター数
};

/**
 * @brief 敵キャラクターの抽象クラス
 */
class Enemy : public Rect {
public:
	// コンストラクタ
	Enemy();
	explicit Enemy(const glm::vec2& _position);
	explicit Enemy(float x, float y);
	explicit Enemy(const glm::vec2& _size, const glm::vec2& _position);

	// デストラクタ
	virtual ~Enemy() {};

	// 敵キャラクターを更新
	virtual void update();

	// 敵キャラクターを描画
	virtual void draw();

	// 左右のスピードを反転する
	virtual void turn();

	// 敵同士の衝突処理
	void intersectEnemy();

	// 敵キャラクターをキルする
	virtual void kill();

	// 敵キャラクターが死んでいるか
	bool isDead() const;

	// 敵キャラクターが生きているか
	bool isAlive() const;

	// 敵キャラクターを描画するか
	bool isVisble() const;

	// 敵キャラクターの種類を取得
	EnemyType getEnemyType() const;
protected:
	// 左に向く
	void turnLeft();

	// 右に向く
	void turnRight();

protected:
	EnemyType	m_enemyType;		//!< 敵キャラクター種類
	bool		m_dead;				//!< 死亡フラグ
	bool		m_visible;			//!< 描画フラグ
	bool		m_falling;			//!< 落下フラグ
	glm::vec2	m_speed;			//!< スピード
	glm::vec2	m_leftPoint;		//!< 左のポイント
	glm::vec2	m_rightPoint;		//!< 右のポイント
	glm::vec2	m_bottomPoints[ENEMY_BOTTOM_COUNT];	//!< 下のポイント
	AnimationController m_animationController;		//!< アニメーションコントローラー
};

/**
 * @brief X軸のスピードを反転する
 * 
 * @param なし
 * 
 * @return なし
 * 
 */
inline void Enemy::turn()
{
	m_speed.x = -m_speed.x;
}

/**
 * @brief 敵キャラクターをキルする
 * 
 * @param なし
 * 
 * @return なし
 * 
 */
inline void Enemy::kill()
{
	m_dead = true;
}

/**
 * @brief 敵キャラクターが死んでいるか
 * 
 */
inline bool Enemy::isDead() const
{
	return m_dead;
}

/**
 * @brief 敵キャラクターが生きているか
 *
 */
inline bool Enemy::isAlive() const
{
	return !m_dead;
}

/**
 * @brief 敵キャラクターを描画するか
 * 
 * @param なし
 * 
 * @return true:描画する、false:描画しない
 * 
 */
inline bool Enemy::isVisble() const
{
	return m_visible;
}

/**
 * @brief 敵キャラクターの種類を取得
 *
 * @param なし
 *
 * @return 敵キャラクターの種類
 *
 */
inline EnemyType Enemy::getEnemyType() const
{
	return m_enemyType;
}

/**
 * @brief 左に向く
 * 
 * @param なし
 * 
 * @return なし
 * 
 */
inline void Enemy::turnLeft()
{
	m_flip = RECT_FLIP_NONE;
	m_speed.x = -m_speed.x;
}

/**
 * @brief 右に向く
 * 
 * @param なし
 *
 * @return なし
 * 
 */
inline void Enemy::turnRight()
{
	m_flip = RECT_FLIP_HORIZONTAL;
	m_speed.x = -m_speed.x;
}
