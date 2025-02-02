#include "EnemyManager.h"
#include "Sprite.h"
#include "world/Part.h"
#include "world/CourseManager.h"
#include "common/font.h"

using namespace glm;
using namespace std;

EnemyManager g_enemyManager;

EnemyManager::EnemyManager()
	: m_pFactory(nullptr)
{
}

EnemyManager::~EnemyManager()
{
	delete m_pFactory;
	m_pFactory = nullptr;

	clear();
}

bool EnemyManager::init()
{
	m_pFactory = new Factory<Enemy>();

	m_pFactory->registerEntity("kuribo", new Creator<Kuribo, Enemy>);
	m_pFactory->registerEntity("nokonoko", new Creator<Nokonoko, Enemy>);
	m_pFactory->registerEntity("koopa", new Creator<Koopa, Enemy>);

	return true;
}

Factory<Enemy>* EnemyManager::getFactory()
{
	return m_pFactory;
}

void EnemyManager::addEnemy(Enemy* _pEnemy)
{
	if (_pEnemy == nullptr)
		return;

	m_enemies.emplace_back(_pEnemy);
}

void EnemyManager::clear()
{
	while (!m_enemies.empty()) {
		delete m_enemies.back();
		m_enemies.pop_back();
	}

	m_updateEnemies.clear();
}

void EnemyManager::koopaKill()
{
	auto result = std::find_if(m_enemies.begin(), m_enemies.end(),
				  [](Enemy* pEnemy) { return pEnemy->getEnemyType() == EnemyType::Koopa; });
	if (result != m_enemies.end()) {
		(*result)->kill();
	}
}

bool EnemyManager::koopaIsDead()
{
	auto result = std::find_if(m_enemies.begin(), m_enemies.end(),
				  [](Enemy* pEnemy) { return pEnemy->getEnemyType() == EnemyType::Koopa; });
	if (result != m_enemies.end()) {
		return (*result)->isDead();
	}

	return false;
}

void EnemyManager::setKoopaRange(const RANGE& _range)
{
	auto result = std::find_if(m_enemies.begin(), m_enemies.end(),
		[](Enemy* pEnemy) { return pEnemy->getEnemyType() == EnemyType::Koopa; });
	if (result != m_enemies.end()) {
		Koopa* koopa = static_cast<Koopa*>(*result);
		koopa->setActionRange(_range);
	}
}

void EnemyManager::hitBlock(const glm::ivec2& _point)
{
	vec2 point = _point * PART_SIZE;
	Rect block(vec2(PART_SIZE, PART_SIZE), point);

	for (const auto& enemy : m_enemies) {
		if (enemy->intersect(block)) {
			enemy->kill();
		}
	}
}

std::vector<Kuribo> EnemyManager::getAllKuribo()
{
	// TODO: return ステートメントをここに挿入します
	std::vector<Kuribo> a;
	return a;
}

std::vector<Nokonoko> EnemyManager::getAllNokonoko()
{
	// TODO: return ステートメントをここに挿入します
	std::vector<Nokonoko> a;
	return a;
}

void EnemyManager::update()
{
	prepare();

	for (const auto& enemy : m_updateEnemies) {
		enemy->update();
	}
}

void EnemyManager::draw()
{
	for (const auto& enemy : m_updateEnemies) {
		enemy->draw();
	}
}

/**
* 敵キャラクター更新準備
* [in]	なし
*/
void EnemyManager::prepare()
{
	// 敵キャラクターの配列をクリア
	m_updateEnemies.clear();

	for (auto& enemy : m_enemies) {
		// 画面内かつ生きている場合追加する
		if (g_courseManager.isInScreen(enemy->getRect()) && enemy->isAlive()) {
			m_updateEnemies.emplace_back(enemy);
		}
	}
}