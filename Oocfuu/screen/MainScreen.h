#pragma once

#include "Screen.h"

struct MainScreen :Screen {

	virtual void init();
	virtual void reset();
	virtual void update();
	virtual void draw();
};