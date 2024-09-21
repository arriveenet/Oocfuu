#pragma once
#include "Rect.h"
#include "animation/AnimationController.h"
#include <glm/glm.hpp>
#include <vector>

#define ENEMY_SPEED			0.5f
#define ENEMY_FALL_SPEED	0.3f
#define ENEMY_BOTTOM_COUNT	2

/**
 * @brief �G�L�����N�^�[�̎��
 */
enum class EnemyType {
	Unknown,	//!< �s��
	Kuribo,		//!< �N���{�[
	Nokonoko,	//!< �m�R�m�R
	Koopa,		//!< �N�b�p
	Max			//!< �G�L�����N�^�[��
};

/**
 * @brief �G�L�����N�^�[�̒��ۃN���X
 */
class Enemy : public Rect {
public:
	// �R���X�g���N�^
	Enemy();
	explicit Enemy(const glm::vec2& _position);
	explicit Enemy(float x, float y);
	explicit Enemy(const glm::vec2& _size, const glm::vec2& _position);

	// �f�X�g���N�^
	virtual ~Enemy() {};

	// �G�L�����N�^�[���X�V
	virtual void update();

	// �G�L�����N�^�[��`��
	virtual void draw();

	// ���E�̃X�s�[�h�𔽓]����
	virtual void turn();

	// �G���m�̏Փˏ���
	void intersectEnemy();

	// �G�L�����N�^�[���L������
	virtual void kill();

	// �G�L�����N�^�[������ł��邩
	bool isDead() const;

	// �G�L�����N�^�[�������Ă��邩
	bool isAlive() const;

	// �G�L�����N�^�[��`�悷�邩
	bool isVisble() const;

	// �G�L�����N�^�[�̎�ނ��擾
	EnemyType getEnemyType() const;
protected:
	// ���Ɍ���
	void turnLeft();

	// �E�Ɍ���
	void turnRight();

protected:
	EnemyType	m_enemyType;		//!< �G�L�����N�^�[���
	bool		m_dead;				//!< ���S�t���O
	bool		m_visible;			//!< �`��t���O
	bool		m_falling;			//!< �����t���O
	glm::vec2	m_speed;			//!< �X�s�[�h
	glm::vec2	m_leftPoint;		//!< ���̃|�C���g
	glm::vec2	m_rightPoint;		//!< �E�̃|�C���g
	glm::vec2	m_bottomPoints[ENEMY_BOTTOM_COUNT];	//!< ���̃|�C���g
	AnimationController m_animationController;		//!< �A�j���[�V�����R���g���[���[
};

/**
 * @brief X���̃X�s�[�h�𔽓]����
 * 
 * @param �Ȃ�
 * 
 * @return �Ȃ�
 * 
 */
inline void Enemy::turn()
{
	m_speed.x = -m_speed.x;
}

/**
 * @brief �G�L�����N�^�[���L������
 * 
 * @param �Ȃ�
 * 
 * @return �Ȃ�
 * 
 */
inline void Enemy::kill()
{
	m_dead = true;
}

/**
 * @brief �G�L�����N�^�[������ł��邩
 * 
 */
inline bool Enemy::isDead() const
{
	return m_dead;
}

/**
 * @brief �G�L�����N�^�[�������Ă��邩
 *
 */
inline bool Enemy::isAlive() const
{
	return !m_dead;
}

/**
 * @brief �G�L�����N�^�[��`�悷�邩
 * 
 * @param �Ȃ�
 * 
 * @return true:�`�悷��Afalse:�`�悵�Ȃ�
 * 
 */
inline bool Enemy::isVisble() const
{
	return m_visible;
}

/**
 * @brief �G�L�����N�^�[�̎�ނ��擾
 *
 * @param �Ȃ�
 *
 * @return �G�L�����N�^�[�̎��
 *
 */
inline EnemyType Enemy::getEnemyType() const
{
	return m_enemyType;
}

/**
 * @brief ���Ɍ���
 * 
 * @param �Ȃ�
 * 
 * @return �Ȃ�
 * 
 */
inline void Enemy::turnLeft()
{
	m_flip = RECT_FLIP_NONE;
	m_speed.x = -m_speed.x;
}

/**
 * @brief �E�Ɍ���
 * 
 * @param �Ȃ�
 *
 * @return �Ȃ�
 * 
 */
inline void Enemy::turnRight()
{
	m_flip = RECT_FLIP_HORIZONTAL;
	m_speed.x = -m_speed.x;
}
