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
	m_enemyFlag = 0;
}

void EnemyManager::update()
{
	// クリボーを更新
	if (m_enemyFlag & ENEMYFLAG_KURIBO) {
		for (vector<Kuribo>::iterator itr = m_kuribos.begin();
			itr != m_kuribos.end();
			itr++) {
			itr->update();
		}
	}

	// ノコノコを更新
	if (m_enemyFlag & ENEMYFLAG_NOKONOKO) {
		for (vector<Nokonoko>::iterator itr = m_nokonokos.begin();
			itr != m_nokonokos.end();
			itr++) {
			itr->update();
		}
	}

	// クッパを更新
	if(m_enemyFlag & ENEMYFLAG_KOOPA)
		m_koopa.update();
}

void EnemyManager::draw()
{
	if (m_enemyFlag & ENEMYFLAG_KURIBO) {
		for (vector<Kuribo>::iterator itr = m_kuribos.begin();
			itr != m_kuribos.end();
			itr++) {
			itr->draw();
		}
	}

	if (m_enemyFlag & ENEMYFLAG_NOKONOKO) {
		for (vector<Nokonoko>::iterator itr = m_nokonokos.begin();
			itr != m_nokonokos.end();
			itr++) {
			itr->draw();
		}
	}

	if (m_enemyFlag & ENEMYFLAG_KOOPA)
		m_koopa.draw();
}