#pragma once
#include "Rect.h"

class Sprite : public Rect{
public:
	Sprite();
	virtual ~Sprite();

	virtual void update();
	virtual void draw();
};