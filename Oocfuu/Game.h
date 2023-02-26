#pragma once
#include "screen/Screen.h"
#include "GameTimer.h"
#include "player/Player.h"

typedef struct tagWORLD {
	unsigned char world;
	unsigned char stage;
} WORLD;

static const WORLD GAME_BEGIN_WORLD = { 1, 4 };

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
private:
	int m_screen;
	Screen* m_pScreens[GAME_SCREEN_MAX];
	bool m_pause;
	unsigned int m_score;
	unsigned int m_topScore;
	unsigned char m_coin;

public:
	Screen* m_pCurrentScreen;
	static unsigned int m_count;
	static bool m_debugInfo;
	WORLD m_world;
	bool m_isGameOver;
	GameTimer m_timer;

	int init();
	void reset();
	void release();
	void update();
	void setScreen(int _screen);
	void drawHUD();
	void countDownTimer();
	void pause();
	bool isPause();
	void addScore(int _score);
	void addCoin();
	void updateTopScore();
	unsigned int getTopScore() const;
};

extern Game g_game;