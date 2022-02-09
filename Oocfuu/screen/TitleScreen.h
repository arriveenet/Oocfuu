#pragma once
#include "Screen.h"
#include "../Course.h"

class TitleScreen : public Screen {
	unsigned int m_texture;
	int m_mode;

public:
	Course m_course;

	TitleScreen();
	virtual void init();
	virtual void reset();
	virtual void update();
	virtual void draw();
};