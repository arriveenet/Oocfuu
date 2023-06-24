#include "Enemy.h"
#include "Game.h"
#include "EnemyManager.h"
#include "world/CourseManager.h"

using namespace glm;
using std::vector;

/**
* �R���X�g���N�^
* @param[in] �Ȃ�
*/
Enemy::Enemy()
	: Enemy(0.0f, 0.0f)
{
}

/**
* �R���X�g���N�^
* @param [in] _position	���W
*/
Enemy::Enemy(const glm::vec2& _position)
	: Enemy(_position.x, _position.y)
{
}

/**
* �R���X�g���N�^
* @param [in] _x	X���W
* @param [in] _y	Y���W
*/
Enemy::Enemy(float _x, float _y)
	: Enemy(glm::vec2(), glm::vec2(_x, _y))
{
}

/**
* �R���X�g���N�^
* @param [in] _size		�T�C�Y
* @param [in] _position	�ʒu
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
* �G�L�����N�^�[���X�V
* @param[in] �Ȃ�
*/
void Enemy::update()
{
	// �n�ʂɂ��Ă��Ȃ���Η�������
	if (m_falling)
		m_speed.y += ENEMY_FALL_SPEED;

	// �����蔻��|�C���g��ݒ肷��
	m_leftPoint = m_position + vec2(0, m_size.y - PART_SIZE);
	m_rightPoint = m_position + vec2(m_size.x, m_size.y - PART_SIZE);
	m_bottomPoints[0] = m_position + m_size;			// �E��
	m_bottomPoints[1] = m_position + vec2(0, m_size.y);	// ����

	// �R�[�X�ƍ��̃|�C���g�Ƃ̓����蔻��
	if (g_courseManager.intersect(m_leftPoint)) {
		vec2 left = (ivec2)m_rightPoint / PART_SIZE * PART_SIZE;
		m_position.x = left.x;
		turnRight();
	}

	// �R�[�X�ƉE�̃|�C���g�Ƃ̓����蔻��
	if (g_courseManager.intersect(m_rightPoint)) {
		vec2 right = (ivec2)m_leftPoint / PART_SIZE * PART_SIZE;
		m_position.x = right.x;
		turnLeft();
	}

	// �n�ʂƂ̓����蔻��
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
* �G�L�����N�^�[��`��
* @param[in] �Ȃ�
*/
void Enemy::draw()
{
	// �f�o�b�O���̃|�C���g��`�悷��
	if (Game::m_debugInfo) {
		// ���E�̃|�C���g��`��
		glColor3ub(0xff, 0xff, 0x00);
		glBegin(GL_POINTS);
		glVertex2fv((const GLfloat*)&m_leftPoint);
		glVertex2fv((const GLfloat*)&m_rightPoint);
		glEnd();

		// ���̃|�C���g��`��
		glColor3ub(0x00, 0xff, 0x00);
		glBegin(GL_POINTS);
		glVertex2fv((const GLfloat*)&m_bottomPoints[0]);
		glVertex2fv((const GLfloat*)&m_bottomPoints[1]);
		glEnd();
		glColor3ub(0xff, 0xff, 0xff);

		// ���C���[�t���[����`��
		Rect::drawWire();
	}
}

/**
* �G�L�����N�^�[���m�̓����蔻��
* @param[in] �Ȃ�
*/
void Enemy::intersectEnemy()
{
	// ��ʓ��̂��ׂĂ̓G�L�����N�^�[���擾
	vector<Enemy*> enemies = g_enemyManager.getAllEnemy();
	for (vector<Enemy*>::iterator enemy = enemies.begin();
		enemy != enemies.end();
		++enemy) {
		//�@�������g�̏ꍇ�R���e�B�j���[
		if (this->operator==(*(*enemy))) {
			continue;
		}

		// ���̓����蔻��
		if ((*enemy)->intersect(m_leftPoint)) {
			m_position.x = (*enemy)->m_position.x + (*enemy)->m_size.x;
			turnRight();
		}

		// �E�̓����蔻��
		if ((*enemy)->intersect(m_rightPoint)) {
			m_position.x = (*enemy)->m_position.x - m_size.x;
			turnLeft();
		}
	}
}