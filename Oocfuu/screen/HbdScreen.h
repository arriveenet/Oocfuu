#pragma once
#include "Screen.h"
#include "world/Course.h"

class HbdScreen :
	public Screen
{
public:

	HbdScreen();
	void init() override;
	void reset() override;
	void update() override;
	void draw() override;
};

