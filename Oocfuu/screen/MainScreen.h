#pragma once
#include "Screen.h"

class MainScreen : public Screen {
	bool m_debugDisplay;

public:
	virtual void init();
	virtual void reset();
	virtual void update();
	virtual void draw();
};