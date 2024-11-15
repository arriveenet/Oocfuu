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
	// クリボー
	case EnemyType::Kuribo:
		m_kuriboes.push_back(dynamic_cast<Kuribo&>(_enemy));
		break;
	// ノコノコ
	case EnemyType::Nokonoko:
		m_nokonokoes.push_back(dynamic_cast<Nokonoko&>(_enemy));
		break;
	// クッパ
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
	// すべてのクリボーをクリア
	m_kuriboes.clear();
	m_kuriboes.shrink_to_fit();

	// すべてのノコノコをクリア
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

	//// クリボーを更新
	//for (vector<Kuribo>::iterator iter = m_kuriboes.begin();
	//	iter != m_kuriboes.end();
	//	++iter) {
	//	iter->update();
	//	iter->intersectEnemy(m_kuriboes);
	//}

	//// ノコノコを更新
	//for (vector<Nokonoko>::iterator iter = m_nokonokoes.begin();
	//	iter != m_nokonokoes.end();
	//	++iter) {
	//	iter->update();
	//}


	// クッパを更新
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

	//// クリボーを描画
	//for (vector<Kuribo>::iterator iter = m_kuriboes.begin();
	//	iter != m_kuriboes.end();
	//	iter++) {
	//	iter->draw();
	//}

	//// ノコノコを描画
	//for (vector<Nokonoko>::iterator iter = m_nokonokoes.begin();
	//	iter != m_nokonokoes.end();
	//	iter++) {
	//	iter->draw();
	//}

	// クッパを描画
	if (m_enemyFlag & ENEMYFLAG_KOOPA)
		m_koopa.draw();
}

/**
* 敵キャラクター更新準備
* [in]	なし
*/
void EnemyManager::prepare()
{
	// 敵キャラクターの配列をクリア
	m_enemies.clear();

	// 更新するクリボーを取得する
	for (vector<Kuribo>::iterator kuribo = m_kuriboes.begin();
		kuribo != m_kuriboes.end();
		++kuribo) {
		// 画面内かつ生きている場合追加する
		if (g_courseManager.isInScreen(kuribo->getRect()) && kuribo->isAlive()) {
			m_enemies.push_back((Enemy*)&(*kuribo));
		}
	}

	// 更新するノコノコーを取得する
	for (vector<Nokonoko>::iterator nokonoko = m_nokonokoes.begin();
		nokonoko != m_nokonokoes.end();
		++nokonoko) {
		// 画面内かつ生きている場合追加する
		if (g_courseManager.isInScreen(nokonoko->getRect()) && nokonoko->isAlive()) {
			m_enemies.push_back((Enemy*)&(*nokonoko));
		}
	}

}