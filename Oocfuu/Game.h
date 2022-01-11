#pragma once
class Game {
public:
	unsigned int m_count;

	void update();
	void drawHUD();
};

extern struct tm currentTime;
extern Game g_game;