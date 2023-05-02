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
	unsigned char id;	// �G�̎��
	unsigned short x;	// �G��X���W
	unsigned short y;	// �G��Y���W
} ENEMY;

// �G�̒��ۃN���X
class Enemy : public Rect, Entity{
public:
	// �R���X�g���N�^
	Enemy();
	explicit Enemy(const glm::vec2& _position);
	explicit Enemy(float x, float y);
	explicit Enemy(const glm::vec2& _size, const glm::vec2& _position);

	// �f�X�g���N�^
	virtual ~Enemy() {};

	// �G�L�����N�^�[���X�V
	virtual void update() override;

	// �G�L�����N�^�[��`��
	virtual void draw() override;

	// ���E�̃X�s�[�h�𔽓]����
	virtual void turn();

	// �G���m�̏Փˏ���
	template <typename T>
	void intersectEnemy(std::vector<T>& _enemies);

	// �G�L�����N�^�[���L������
	virtual void kill();

	// �G�L�����N�^�[������ł��邩
	bool isDead();

	// �G�L�����N�^�[��`�悷�邩
	bool isVisble();

protected:
	glm::vec2	m_speed;	// �X�s�[�h
	bool		m_dead;		// ���S�t���O
	bool		m_turned;	// �^�[���t���O
	bool		m_visible;	// �`��t���O
	AnimationController m_animationController;	// �A�j���[�V�����R���g���[���[
};

/**
* �R���X�g���N�^
* @param[in] �Ȃ�
*/
inline Enemy::Enemy()
	: Enemy(0.0f, 0.0f)
{
}

/**
* �R���X�g���N�^
* @param [in] _position	���W
*/
inline Enemy::Enemy(const glm::vec2& _position)
	: Enemy(_position.x, _position.y)
{
}

/**
* �R���X�g���N�^
* @param [in] _x	X���W
* @param [in] _y	Y���W
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
* �R���X�g���N�^
* @param [in] _size		�T�C�Y
* @param [in] _position	�ʒu
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
* �G�L�����N�^�[���X�V
* @param[in] �Ȃ�
*/
inline void Enemy::update()
{
}

/**
* �G�L�����N�^�[��`��
* @param[in] �Ȃ�
*/
inline void Enemy::draw()
{
	Rect::draw();
}

/**
* X���̃X�s�[�h�𔽓]����
*/
inline void Enemy::turn()
{
	m_speed.x = -m_speed.x;
}

/**
* �G���m�̏Փˏ���
* @param[in] _enemies	�G�̔z��
*/
template<typename T>
inline void Enemy::intersectEnemy(std::vector<T>& _enemies)
{
	std::vector<T>::template iterator iter = _enemies.begin();
	for (; iter != _enemies.end(); ++iter) {
		//�@�������g�������̓^�[���ςݏꍇ�R���e�B�j���[
		if (this->operator==(*iter))
			continue;

		// ���̓G�Ƃ̓����蔻��
		if (intersect(*iter)) {
			m_turned = true;
			turn();
		}
	}
}

/**
* �G�L�����N�^�[���L������
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
