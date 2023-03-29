#pragma once
#include "Screen.h"
#include "world/Course.h"
#include <string>

class TitleScreen : public Screen {
	int m_mode;
	std::string m_command;

public:

	TitleScreen();
	void init() override;
	void reset() override;
	void update() override;
	void draw() override;
};