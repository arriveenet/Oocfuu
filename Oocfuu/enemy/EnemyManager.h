#pragma once
#include "Enemy.h"
#include "Kuribo.h"
#include "Nokonoko.h" 
#include "Koopa.h"
#include "common/range.h"
#include "Factory.h"

#include <vector>
#include <glm/glm.hpp>

class EnemyManager {
public:
	EnemyManager();
	virtual ~EnemyManager();

	bool init();
	Factory<Enemy>* getFactory();;
	void addEnemy(Enemy* _pEnemy);
	void clear();
	void koopaKill();
	bool koopaIsDead();
	void setKoopaRange(const RANGE& _range);
	void hitBlock(const glm::ivec2& _point);
	std::vector<Kuribo> getAllKuribo();
	std::vector<Nokonoko> getAllNokonoko();
	std::vector<Enemy*>& getAllEnemy() { return m_updateEnemies; }
	size_t getEnemySize()const { return m_updateEnemies.size(); }

	void update();
	void draw();

protected:
	void prepare();

private:
	std::vector<Enemy*> m_enemies;
	std::vector<Enemy*> m_updateEnemies;
	Factory<Enemy>* m_pFactory;
};

extern EnemyManager g_enemyManager;