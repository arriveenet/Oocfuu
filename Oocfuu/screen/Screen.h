#pragma once

struct Screen{
	virtual void init() = 0;
	virtual void reset() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
};