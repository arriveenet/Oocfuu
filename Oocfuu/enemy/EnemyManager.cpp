#include "EnemyManager.h"

using namespace std;

EnemyManager g_enemyManager;

EnemyManager::EnemyManager()
	: m_enemyFlag(0)
{
}

void EnemyManager::addEnemy(const ENEMYINFO& _info)
{
	switch (_info.type) {
	case ENEMYTYPE_KURIBO:
		m_kuribos.push_back(Kuribo(_info.position));
		break;
	case ENEMYTYPE_NOKONIKO:
		m_nokonokos.push_back(Nokonoko(_info.position));
		break;
	case ENEMYTYPE_KOOPA:
		m_koopa.m_position = _info.position;
		break;
	}
}

void EnemyManager::reset()
{
	m_kuribos.clear();
	m_nokonokos.clear();
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

void EnemyManager::update()
{
	// クリボーを更新
	if (m_enemyFlag & ENEMYFLAG_KURIBO) {
		for (vector<Kuribo>::iterator iter = m_kuribos.begin();
			iter != m_kuribos.end();
			++iter) {
			iter->update();
		}
	}

	// ノコノコを更新
	if (m_enemyFlag & ENEMYFLAG_NOKONOKO) {
		for (vector<Nokonoko>::iterator iter = m_nokonokos.begin();
			iter != m_nokonokos.end();
			++iter) {
			iter->update();
		}
	}

	// クッパを更新
	if(m_enemyFlag & ENEMYFLAG_KOOPA)
		m_koopa.update();
}

void EnemyManager::draw()
{
	if (m_enemyFlag & ENEMYFLAG_KURIBO) {
		for (vector<Kuribo>::iterator iter = m_kuribos.begin();
			iter != m_kuribos.end();
			iter++) {
			iter->draw();
		}
	}

	if (m_enemyFlag & ENEMYFLAG_NOKONOKO) {
		for (vector<Nokonoko>::iterator iter = m_nokonokos.begin();
			iter != m_nokonokos.end();
			iter++) {
			iter->draw();
		}
	}

	if (m_enemyFlag & ENEMYFLAG_KOOPA)
		m_koopa.draw();
}