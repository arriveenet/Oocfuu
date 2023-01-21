#pragma once
#include "screen/Screen.h"

#define GAME_START_TIME		400
#define GAME_TIMER_LATE		32

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
	short m_time;
	bool m_isTimerUpdate;
	bool m_visibleTimer;
	bool m_pause;

public:
	Screen* m_pCurrentScreen;
	static unsigned int m_count;
	static bool m_debugInfo;
	unsigned int m_score;
	unsigned char m_coin;
	WORLD m_world;
	bool m_isGameOver;

	int init();
	void release();
	void update();
	void setScreen(int _screen);
	void drawHUD();
	void startTimer() { m_isTimerUpdate = true; };
	void stopTimer() { m_isTimerUpdate = false; };
	void resetTimer() { m_time = GAME_START_TIME; };
	void visibleTimer(bool _visible) { m_visibleTimer = _visible; };
	void pause();
	bool isPause() { return m_pause; };
};

extern Game g_game;