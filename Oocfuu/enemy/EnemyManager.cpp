#include "EnemyManager.h"
#include "Sprite.h"
#include "world/Part.h"
#include "world/CourseManager.h"
#include "common/font.h"

using namespace glm;
using namespace std;

EnemyManager g_enemyManager;

EnemyManager::EnemyManager()
	: m_enemyFlag(0)
{
}

EnemyManager::~EnemyManager()
{
	clear();
}

void EnemyManager::addEnemy(Enemy& _enemy)
{
	switch (_enemy.getEnemyType()) {
	// �N���{�[
	case EnemyType::Kuribo:
		m_kuriboes.push_back(dynamic_cast<Kuribo&>(_enemy));
		break;
	// �m�R�m�R
	case EnemyType::Nokonoko:
		m_nokonokoes.push_back(dynamic_cast<Nokonoko&>(_enemy));
		break;
	// �N�b�p
	case EnemyType::Koopa:
		m_enemyFlag |= ENEMYFLAG_KOOPA;
		m_koopa.setPosition(_enemy.getPosition());
		break;
	default:
		assert(false);
		break;
	}
}

void EnemyManager::clear()
{
	// ���ׂẴN���{�[���N���A
	m_kuriboes.clear();
	m_kuriboes.shrink_to_fit();

	// ���ׂẴm�R�m�R���N���A
	m_nokonokoes.clear();
	m_nokonokoes.shrink_to_fit();

	m_koopa.reset();

	m_enemyFlag = 0;
}

void EnemyManager::koopaKill()
{
	if (m_enemyFlag & ENEMYFLAG_KOOPA)
		m_koopa.kill();
}

bool EnemyManager::koopaIsDead()
{
	return m_koopa.isDead();
}

void EnemyManager::setKoopaRange(const RANGE& _range)
{
	m_koopa.setActionRange(_range);
}

void EnemyManager::hitBlock(const glm::ivec2& _point)
{
	vec2 point = _point * PART_SIZE;
	Rect block(vec2(PART_SIZE, PART_SIZE), point);

	for (vector<Kuribo>::iterator iter = m_kuriboes.begin();
		iter != m_kuriboes.end();
		++iter) {
		if (iter->intersect(block)) {
			iter->kill();
		}
	}

}


void EnemyManager::update()
{
	prepare();

	for (vector<Enemy*>::iterator enemy = m_enemies.begin();
		enemy != m_enemies.end();
		++enemy) {
		(*enemy)->update();
	}

	//// �N���{�[���X�V
	//for (vector<Kuribo>::iterator iter = m_kuriboes.begin();
	//	iter != m_kuriboes.end();
	//	++iter) {
	//	iter->update();
	//	iter->intersectEnemy(m_kuriboes);
	//}

	//// �m�R�m�R���X�V
	//for (vector<Nokonoko>::iterator iter = m_nokonokoes.begin();
	//	iter != m_nokonokoes.end();
	//	++iter) {
	//	iter->update();
	//}


	// �N�b�p���X�V
	if(m_enemyFlag & ENEMYFLAG_KOOPA)
		m_koopa.update();

}

void EnemyManager::draw()
{
	prepare();

	for (vector<Enemy*>::iterator enemy = m_enemies.begin();
		enemy != m_enemies.end();
		++enemy) {
		(*enemy)->draw();
	}

	//// �N���{�[��`��
	//for (vector<Kuribo>::iterator iter = m_kuriboes.begin();
	//	iter != m_kuriboes.end();
	//	iter++) {
	//	iter->draw();
	//}

	//// �m�R�m�R��`��
	//for (vector<Nokonoko>::iterator iter = m_nokonokoes.begin();
	//	iter != m_nokonokoes.end();
	//	iter++) {
	//	iter->draw();
	//}

	// �N�b�p��`��
	if (m_enemyFlag & ENEMYFLAG_KOOPA)
		m_koopa.draw();
}

/**
* �G�L�����N�^�[�X�V����
* [in]	�Ȃ�
*/
void EnemyManager::prepare()
{
	// �G�L�����N�^�[�̔z����N���A
	m_enemies.clear();

	// �X�V����N���{�[���擾����
	for (vector<Kuribo>::iterator kuribo = m_kuriboes.begin();
		kuribo != m_kuriboes.end();
		++kuribo) {
		// ��ʓ��������Ă���ꍇ�ǉ�����
		if (g_courseManager.isInScreen(kuribo->getRect()) && kuribo->isAlive()) {
			m_enemies.push_back((Enemy*)&(*kuribo));
		}
	}

	// �X�V����m�R�m�R�[���擾����
	for (vector<Nokonoko>::iterator nokonoko = m_nokonokoes.begin();
		nokonoko != m_nokonokoes.end();
		++nokonoko) {
		// ��ʓ��������Ă���ꍇ�ǉ�����
		if (g_courseManager.isInScreen(nokonoko->getRect()) && nokonoko->isAlive()) {
			m_enemies.push_back((Enemy*)&(*nokonoko));
		}
	}

}