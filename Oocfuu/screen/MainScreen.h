#pragma once
#include "Screen.h"
#include "../enemy/EnemyManager.h"

class MainScreen : public Screen {
	bool m_debugDisplay;
	EnemyManager m_enemyManager;

public:
	bool m_pause;

	virtual void init();
	virtual void reset();
	virtual void update();
	virtual void draw();
};