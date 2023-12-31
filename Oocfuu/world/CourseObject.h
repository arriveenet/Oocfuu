#pragma once
#include "Sprite.h"

class Kinopio : public Sprite {
public:
	Kinopio();
	Kinopio(float x, float y);

	void draw() override;
};

class Cake : public Sprite {
public:
	Cake();
	Cake(float x, float y);

	void draw() override;
};