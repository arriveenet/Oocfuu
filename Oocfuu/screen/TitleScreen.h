#pragma once
#include "Screen.h"
#include "world/Course.h"
#include <string>

class TitleScreen : public Screen {
	int m_mode;
	std::string m_command;

public:

	TitleScreen();
	virtual void init();
	virtual void reset();
	virtual void update();
	virtual void draw();
};