#pragma once
#include "Rect.h"

#define LIFT_SIZE		8

class Lift : public Rect{
	int m_width;

public:
	Lift(float _x, float _y, int _width);

	void update();
	void draw();
};

