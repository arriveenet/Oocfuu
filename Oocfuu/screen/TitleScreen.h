#pragma once
#include "Screen.h"
#include "../Course.h"

class TitleScreen : public Screen {
	int m_mode;

public:

	TitleScreen();
	virtual void init();
	virtual void reset();
	virtual void update();
	virtual void draw();
};