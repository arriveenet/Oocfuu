#pragma once
#include "Screen.h"
#include "../Course.h"

class TitleScreen : public Screen {
	unsigned int m_texture;
	int m_mode;

public:

	TitleScreen();
	virtual void init();
	virtual void reset();
	virtual void update();
	virtual void draw();
};