#pragma once
#include "Rect.h"

class Kinopio : public Rect{
public:
	bool m_enable;

	Kinopio();
	~Kinopio();

	void draw() override;
};