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
		break;
	case ENEMYTYPE_KOOPA:
		m_koopa.m_position = _info.position;
		break;
	}
}

void EnemyManager::reset()
{
	m_kuribos.clear();
	m_enemyFlag = 0;
}

void EnemyManager::update()
{
	if (m_enemyFlag & ENEMYFLAG_KURIBO) {
		for (vector<Kuribo>::iterator itr = m_kuribos.begin();
			itr != m_kuribos.end();
			itr++) {
			Kuribo* kuribo = &(*itr);
			kuribo->update();
		}
	}

	if(m_enemyFlag & ENEMYFLAG_KOOPA)
		m_koopa.update();
}

void EnemyManager::draw()
{
	if (m_enemyFlag & ENEMYFLAG_KURIBO) {
		for (vector<Kuribo>::iterator itr = m_kuribos.begin();
			itr != m_kuribos.end();
			itr++) {
			Kuribo* kuribo = &(*itr);
			kuribo->draw();
		}
	}

	if (m_enemyFlag & ENEMYFLAG_KOOPA)
		m_koopa.draw();
}