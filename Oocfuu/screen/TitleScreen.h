#pragma once

#include "Screen.h"

struct TitleScreen:Screen{
	int m_counter;

	virtual void init();
	virtual void reset();
	virtual void update();
	virtual void draw();
};