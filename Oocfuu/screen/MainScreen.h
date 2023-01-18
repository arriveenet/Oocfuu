#pragma once
#include "Screen.h"
#include "../enemy/EnemyManager.h"

class MainScreen : public Screen {
public:
	bool m_pause;

	virtual void init();
	virtual void reset();
	virtual void update();
	virtual void draw();
};