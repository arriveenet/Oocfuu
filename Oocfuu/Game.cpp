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

#include <algorithm>
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
	m_world = GAME_BEGIN_WORLD;
	m_debugInfo = false; 
	m_pause = false;

	return 0;
}

void Game::reset()
{
	m_count = 0;
	m_isGameOver = false;
	m_score = 0;
	m_coin = 0;
	m_world = GAME_BEGIN_WORLD;
	m_pause = false;
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

	m_timer.update();
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
		if (m_timer.getVisible()) {
			fontPosition(208, 24);
			//fontDraw("%02d:%02d", g_app.m_currentTime.tm_hour, g_app.m_currentTime.tm_min);
			fontDraw("%03d", m_timer.getTime());
		}
	}
	fontEnd();
}

void Game::countDownTimer()
{
}

void Game::pause()
{
	// ポーズ音が再生中であればポーズしない
	if (g_pSound->getState(SOUND_SE_PAUSE) == AL_PLAYING)
		return;

	m_pause = m_pause ? false : true;
	g_pSound->play(SOUND_SE_PAUSE);
}

bool Game::isPause()
{
	return m_pause;
}

void Game::addScore(int _score)
{
	m_score += _score;
}

void Game::updateTopScore()
{
	m_topScore = std::max(m_score, m_topScore);
}

unsigned int Game::getTopScore() const
{
	return m_topScore;
}