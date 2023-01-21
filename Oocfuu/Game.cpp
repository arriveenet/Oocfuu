#include "Game.h"

#include "App.h"
#include "font.h"
#include "Rect.h"
#include "TextureManager.h"
#include "screen/TitleScreen.h"
#include "screen/IntroScreen.h"
#include "screen/MainScreen.h"
#include "screen/HbdScreen.h"
#include "screen/TpScreen.h"
#include "screen/GoodbyeOocfuuScreen.h"
#include "sound/Sound.h"

#include <freeglut.h>

Game g_game;
unsigned int Game::m_count;
bool Game::m_debugInfo;

int Game::init()
{
	m_pScreens[GAME_SCREEN_TITLE] = new TitleScreen();
	m_pScreens[GAME_SCREEN_INTRO] = new IntroScreen();
	m_pScreens[GAME_SCREEN_MAIN] = new MainScreen();
	m_pScreens[GAME_SCREEN_HBD] = new HbdScreen();
	m_pScreens[GAME_SCREEN_TP] = new TpScreen();
	m_pScreens[GAME_SCREEN_GO] = new GoodbyeOocfuuScreen();

	for (int i = 0; i < GAME_SCREEN_MAX; i++)
		m_pScreens[i]->init();

	m_screen = GAME_SCREEN_TITLE;
	m_pCurrentScreen = m_pScreens[m_screen];
	m_count = 0;
	m_isGameOver = false;
	m_score = 0;
	m_coin = 0;
	m_world = GAME_START_WORLD;
	m_time = GAME_START_TIME;
	m_isTimerUpdate = false;
	m_visibleTimer = false;
	m_debugInfo = false; 
	m_pause = false;

	return 0;
}

void Game::release()
{
	for (int i = 0; i < GAME_SCREEN_MAX; i++) {
		delete m_pScreens[i];
		m_pScreens[i] = NULL;
	}
}

void Game::update()
{
	if (m_pause)
		return;

	m_count++;

	// タイマーをカウントダウンさせる
	if (m_isTimerUpdate) {
		static int lastTime;
		int time = m_count / GAME_TIMER_LATE;
		if ((time != lastTime) && (m_time > 0)) {
			//printf("time=%d\n", time);
			m_time--;
		}
		lastTime = time;
	}
}

void Game::setScreen(int _screen) {
	m_pCurrentScreen = m_pScreens[m_screen = _screen];
	m_pCurrentScreen->reset();
}

void Game::drawHUD()
{
	fontBegin();
	{
		fontPosition(24, 16);
		fontDraw("OOCFUU");

		fontPosition(24, 24);
		fontDraw("%06d", m_score);
		//fontDraw("%06d", g_app.m_currentTime.tm_year + 1900);

		unsigned char hudCoin = 'c';
		const int len = 6;
		unsigned char table[len] = { 0,1,2,2,1,0 };
		hudCoin += table[(m_count / 8) % len];

		fontPosition(88, 24);
		//fontDraw("%cx%02d", hudCoin, g_app.m_currentTime.tm_sec);
		fontDraw("%cx%02d", hudCoin, m_coin);

		fontPosition(144, 16);
		fontDraw("WORLD");

		fontPosition(144, 24);
		fontDraw("%2d-%d", m_world.world, m_world.stage);
		//fontDraw("%2d-%d", g_app.m_currentTime.tm_mon + 1, g_app.m_currentTime.tm_mday);
		//fontDraw("%2d-%d", 2, 4);

		fontPosition(200, 16);
		fontDraw("TIME");
		// タイム表示を表示する
		if (m_visibleTimer) {
			fontPosition(208, 24);
			//fontDraw("%02d:%02d", g_app.m_currentTime.tm_hour, g_app.m_currentTime.tm_min);
			fontDraw("%03d", m_time);
		}
	}
	fontEnd();
}

void Game::pause()
{
	printf("sound state=0x%x\n", g_pSound->getState(SOUND_SE_PAUSE));
	// ポーズ音が再生中であればポーズしない
	if (g_pSound->getState(SOUND_SE_PAUSE) == AL_PLAYING)
		return;

	m_pause = m_pause ? false : true;
	g_pSound->play(SOUND_SE_PAUSE);
}
