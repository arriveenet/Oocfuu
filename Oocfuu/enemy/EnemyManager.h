#pragma once
#include "Enemy.h"
#include "Kuribo.h"
#include "Nokonoko.h"
#include "Koopa.h"
#include "common/range.h"

#include <vector>
#include <glm/glm.hpp>

/* 敵キャラクターフラグ */
#define ENEMYFLAG_KURIBO		0x01
#define ENEMYFLAG_NOKONOKO		0x02
#define ENEMYFLAG_KOOPA			0x04

typedef struct tagENEMYINFO {
	ENEMYTYPE type;
	glm::ivec2 position;
} ENEMYINFO;

class EnemyManager {
	std::vector<Kuribo> m_kuriboes;
	std::vector<Nokonoko> m_nokonokoes;
	std::vector<Enemy*> m_enemies;
	Koopa m_koopa;

public:
	int m_enemyFlag;

	EnemyManager();
	virtual ~EnemyManager();

	void addEnemy(const ENEMYINFO& _info);
	void clear();
	void koopaKill();
	bool koopaIsDead();
	void setKoopaRange(const RANGE& _range);
	void hitBlock(const glm::ivec2& _point);
	std::vector<Kuribo>& getAllKuribo() { return m_kuriboes; }
	std::vector<Nokonoko>& getAllNokonoko() { return m_nokonokoes; }
	std::vector<Enemy*>& getAllEnemy() { return m_enemies; }
	size_t getEnemySize()const { return m_enemies.size(); }

	void update();
	void draw();

protected:
	void prepare();
};

extern EnemyManager g_enemyManager;