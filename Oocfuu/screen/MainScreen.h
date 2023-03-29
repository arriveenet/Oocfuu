#pragma once
#include "Screen.h"
#include "enemy/EnemyManager.h"

class MainScreen : public Screen {
public:
	void init() override;
	void reset() override;
	void update() override;
	void draw() override;
};