#pragma once
#include "screen/Screen.h"

enum {
	GAME_SCREEN_TITLE,
	GAME_SCREEN_INTRO,
	GAME_SCREEN_MAIN,
	GAME_SCREEN_HBD,
	GAME_SCREEN_TP,
	GAME_SCREEN_GO,
	GAME_SCREEN_MAX
};

class Game {
	int m_screen;
	Screen* m_pScreens[GAME_SCREEN_MAX];

public:
	Screen* m_pCurrentScreen;
	unsigned int m_count;

	int init();
	void update();
	void setScreen(int _screen);
	void drawHUD();
};

extern Game g_game;