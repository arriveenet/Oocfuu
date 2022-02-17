#include "Game.h"
#include "font.h"
#include "Rect.h"
#include "Texture.h"
#include "screen/TitleScreen.h"
#include "screen/IntroScreen.h"
#include "screen/MainScreen.h"
#include "screen/HbdScreen.h"
#include "screen/TpScreen.h"
#include "screen/GoodbyeOocfuuScreen.h"

#include <freeglut.h>

struct tm currentTime;
Game g_game;

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

	return 0;
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
	GLuint texture = g_textures[TEXTURE_COIN_1].m_texture;
	glBindTexture(
		GL_TEXTURE_2D,	// GLenum target
		texture);		// GLuint texture
	Rect rect(vec2(8, 8), vec2(g_course.m_scroll + 88, 24));
	rect.draw();

	fontBegin();
	{
		fontPosition(24, 16);
		fontDraw("OOCFUU");
		fontPosition(24, 24);
		fontDraw("%06d", currentTime.tm_year + 1900);
		fontPosition(96, 24);
		fontDraw("x%02d", currentTime.tm_sec);
		fontPosition(144, 16);
		fontDraw("WORLD");
		fontPosition(152, 24);
		fontDraw("%d-%d", currentTime.tm_mon + 1, currentTime.tm_mday);
		fontPosition(200, 16);
		fontDraw("TIME");
		fontPosition(200, 24);
		fontDraw("%02d:%02d", currentTime.tm_hour, currentTime.tm_min);
		fontPosition(40, 64);
	}
	fontEnd();
}