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

#include <freeglut.h>

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

Game g_game;
unsigned int Game::m_count;

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
	m_world = { 1,1 };

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
	m_count++;
}

void Game::setScreen(int _screen) {
	m_pCurrentScreen = m_pScreens[m_screen = _screen];
	m_pCurrentScreen->reset();
}

void Game::drawHUD()
{
	//GLuint texture = g_textures[TEXTURE_COIN_1].m_texture;
	//glBindTexture(
	//	GL_TEXTURE_2D,	// GLenum target
	//	texture);		// GLuint texture
	//Rect rect(vec2(8, 8), vec2(g_course.m_scroll + 88, 24));
	//rect.draw();

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
		fontDraw("%2d-%d", m_world.major, m_world.miner);
		//fontDraw("%2d-%d", g_app.m_currentTime.tm_mon + 1, g_app.m_currentTime.tm_mday);
		//fontDraw("%2d-%d", 2, 4);

		fontPosition(200, 16);
		fontDraw("TIME");

		fontPosition(200, 24);
		fontDraw("%02d:%02d", g_app.m_currentTime.tm_hour, g_app.m_currentTime.tm_min);
		//fontDraw("00:00");
	}
	fontEnd();
}