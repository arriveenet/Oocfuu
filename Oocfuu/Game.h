#pragma once
#include "Screen.h"

enum {
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

	int init();
	void setScreen(int _screen);
	void drawHUD();
};

extern struct tm currentTime;
extern Game g_game;