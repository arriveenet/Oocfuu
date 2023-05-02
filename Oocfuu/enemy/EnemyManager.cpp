#include "EnemyManager.h"
#include "world/Part.h"

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

void EnemyManager::addEnemy(const ENEMYINFO& _info)
{
	switch (_info.type) {
	case ENEMYTYPE_KURIBO:
	{
		Kuribo* kuribo = new Kuribo(_info.position);
		m_kuribos.push_back(kuribo);
	}
		break;
	case ENEMYTYPE_NOKONIKO:
		m_nokonokos.push_back(Nokonoko(_info.position));
		break;
	case ENEMYTYPE_KOOPA:
		m_koopa.m_position = _info.position;
		break;
	}
}

void EnemyManager::clear()
{
	for (vector<Kuribo*>::iterator iter = m_kuribos.begin();
		iter != m_kuribos.end();
		++iter) {
		delete *iter;
	}

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

void EnemyManager::hitBlock(const glm::ivec2& _point)
{
	vec2 point = _point * PART_SIZE;
	Rect block(vec2(PART_SIZE, PART_SIZE), point);

	if (m_enemyFlag & ENEMYFLAG_KURIBO) {
		for (vector<Kuribo*>::iterator iter = m_kuribos.begin();
			iter != m_kuribos.end();
			++iter) {
			if ((*iter)->intersect(block)) {
				(*iter)->kill();
			}
		}
	}
}

std::vector<Kuribo*> EnemyManager::getUpdateAllKuribos()
{
	std::vector<Kuribo*> updateKuribos;
	for (vector<Kuribo*>::iterator iter = m_kuribos.begin();
		iter != m_kuribos.end();
		++iter) {
		if (!(*iter)->isDead()) {
			updateKuribos.push_back(*iter);
		}
	}

	return updateKuribos;
}

void EnemyManager::update()
{
	vector<Enemy*> enemies;

	// クリボーを更新
	if (m_enemyFlag & ENEMYFLAG_KURIBO) {
		for (vector<Kuribo*>::iterator iter = m_kuribos.begin();
			iter != m_kuribos.end();
			++iter) {
			(*iter)->update();

			// 描画するクリボーを追加する
			if ((*iter)->isVisble()) {
				enemies.push_back(*iter);
			}
		}
	}

	// ノコノコを更新
	if (m_enemyFlag & ENEMYFLAG_NOKONOKO) {
		for (vector<Nokonoko>::iterator nokonoko = m_nokonokos.begin();
			nokonoko != m_nokonokos.end();
			++nokonoko) {
			nokonoko->update();
		}
	}

	// クッパを更新
	if(m_enemyFlag & ENEMYFLAG_KOOPA)
		m_koopa.update();

	// 必要な敵キャラクターを更新
	for (vector<Enemy*>::iterator iter = enemies.begin();
		iter != enemies.end();
		++iter) {

	}
}

void EnemyManager::draw()
{
	if (m_enemyFlag & ENEMYFLAG_KURIBO) {
		for (vector<Kuribo*>::iterator iter = m_kuribos.begin();
			iter != m_kuribos.end();
			iter++) {
			(*iter)->draw();
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