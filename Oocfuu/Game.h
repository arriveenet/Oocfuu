#pragma once
#include "screen/Screen.h"

typedef struct tagWORLD {
	unsigned char world;
	unsigned char stage;
} WORLD;

static const WORLD GAME_START_WORLD = { 1, 1 };

enum GAME_SCREEN {
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
	static unsigned int m_count;
	unsigned int m_score;
	unsigned char m_coin;
	WORLD m_world;
	bool m_isGameOver;

	int init();
	void release();
	void update();
	void setScreen(int _screen);
	void drawHUD();
};

extern Game g_game;