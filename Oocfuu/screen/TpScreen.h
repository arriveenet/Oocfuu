#pragma once
#include "Screen.h"

class TpScreen :
	public Screen
{
	unsigned int m_count = 0;
public:
	void init() override;
	void reset() override;
	void update() override;
	void draw() override;
};

