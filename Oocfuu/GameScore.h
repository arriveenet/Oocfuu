#pragma once
#include "Rect.h"

class GameScore : public Rect{
public:
	GameScore();
	virtual ~GameScore();
	
	void start(float _x, float _y);
	void update();
	void draw();
};

