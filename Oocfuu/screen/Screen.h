#pragma once

class Game;

struct Screen
{
	Screen();
	virtual ~Screen();

	virtual void init() = 0;
	virtual void reset() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;

	Game* m_pGame;
};