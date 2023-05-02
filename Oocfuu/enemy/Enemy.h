#pragma once
#include "Rect.h"
#include "common/Entity.h"
#include "animation/AnimationController.h"
#include <glm/glm.hpp>
#include <vector>

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
	template <typename T>
	void intersectEnemy(std::vector<T>& _enemies);

	// 敵キャラクターをキルする
	virtual void kill();

	// 敵キャラクターが死んでいるか
	bool isDead();

	// 敵キャラクターを描画するか
	bool isVisble();

protected:
	glm::vec2	m_speed;	// スピード
	bool		m_dead;		// 死亡フラグ
	bool		m_turned;	// ターンフラグ
	bool		m_visible;	// 描画フラグ
	AnimationController m_animationController;	// アニメーションコントローラー
};

/**
* コンストラクタ
* @param[in] なし
*/
inline Enemy::Enemy()
	: Enemy(0.0f, 0.0f)
{
}

/**
* コンストラクタ
* @param [in] _position	座標
*/
inline Enemy::Enemy(const glm::vec2& _position)
	: Enemy(_position.x, _position.y)
{
}

/**
* コンストラクタ
* @param [in] _x	X座標
* @param [in] _y	Y座標
*/
inline Enemy::Enemy(float _x, float _y)
	: m_animationController()
	, m_speed()
	, m_dead(false)
	, m_turned(false)
	, m_visible(false)
	, Rect(_x, _y)
{
}

/**
* コンストラクタ
* @param [in] _size		サイズ
* @param [in] _position	位置
*/
inline Enemy::Enemy(const glm::vec2& _size, const glm::vec2& _position)
	: m_animationController()
	, m_speed()
	, m_dead(false)
	, m_turned(false)
	, m_visible(false)
	, Rect(_size, _position)
{
}

/**
* 敵キャラクターを更新
* @param[in] なし
*/
inline void Enemy::update()
{
}

/**
* 敵キャラクターを描画
* @param[in] なし
*/
inline void Enemy::draw()
{
	Rect::draw();
}

/**
* X軸のスピードを反転する
*/
inline void Enemy::turn()
{
	m_speed.x = -m_speed.x;
}

/**
* 敵同士の衝突処理
* @param[in] _enemies	敵の配列
*/
template<typename T>
inline void Enemy::intersectEnemy(std::vector<T>& _enemies)
{
	std::vector<T>::template iterator iter = _enemies.begin();
	for (; iter != _enemies.end(); ++iter) {
		//　自分自身もしくはターン済み場合コンティニュー
		if (this->operator==(*iter))
			continue;

		// 他の敵との当たり判定
		if (intersect(*iter)) {
			m_turned = true;
			turn();
		}
	}
}

/**
* 敵キャラクターをキルする
*/
inline void Enemy::kill()
{
	m_dead = true;
}

inline bool Enemy::isDead()
{
	return m_dead;
}

inline bool Enemy::isVisble()
{
	return m_visible;
}
