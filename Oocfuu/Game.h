#pragma once
#include "screen/Screen.h"

#define GAME_START_TIME		400
#define GAME_TIMER_LATE		32

typedef struct tagWORLD {
	unsigned char world;
	unsigned char stage;
} WORLD;

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
	short m_time;
	bool m_isTimerUpdate;

public:
	Screen* m_pCurrentScreen;
	static unsigned int m_count;
	unsigned int m_score;
	unsigned char m_coin;
	WORLD m_world;
	bool m_isGameOver;
	bool m_visibleTimer;

	int init();
	void release();
	void update();
	void setScreen(int _screen);
	void drawHUD();
	void startTimer() { m_isTimerUpdate = true; };
	void stopTimer() { m_isTimerUpdate = false; };
	void resetTimer() { m_time = GAME_START_TIME; };
	void visibleTimer(bool _visible) { m_visibleTimer = _visible; };
};

extern Game g_game;