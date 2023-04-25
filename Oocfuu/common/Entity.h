#pragma once

struct Entity {
	virtual void update() = 0;
	virtual void draw() = 0;
};