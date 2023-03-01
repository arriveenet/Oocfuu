#pragma once
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

enum ENEMYTYPE {
	ENEMYTYPE_KURIBO,
	ENEMYTYPE_NOKONIKO,
	ENEMYTYPE_KOOPA,
	ENEMYTYPE_MAX
};

typedef struct tagENEMYINFO {
	ENEMYTYPE type;
	glm::ivec2 position;
} ENEMYINFO;

class EnemyManager {
	std::vector<Kuribo> m_kuribos;
	std::vector<Nokonoko> m_nokonokos;
	Koopa m_koopa;

public:
	int m_enemyFlag;

	EnemyManager();

	void addEnemy(const ENEMYINFO& _info);
	void reset();
	void koopaKill();
	bool koopaIsDead();
	void setKoopaRange(const RANGE& _range);

	void update();
	void draw();
};

extern EnemyManager g_enemyManager;