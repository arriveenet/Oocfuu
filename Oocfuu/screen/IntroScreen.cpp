#include "IntroScreen.h"
#include "../Game.h"
#include "../Keyboard.h"
#include "../font.h"
#include "../Player/Player.h"
#include "../Course.h"

using namespace glm;

void IntroScreen::init()
{
	m_counter = 0;
}

void IntroScreen::reset()
{
	m_counter = 0;
	glClearColor({ 0 }, { 0 }, { 0 }, { 1 });
}

void IntroScreen::update()
{
	if (++m_counter > 60 * 3)
		g_game.setScreen(GAME_SCREEN_MAIN);
			
}

void IntroScreen::draw()
{
	g_game.drawHUD();
	/*
	if (g_game.isGameOver()){
		fontBegin();
		fontPosition(88, 112);
		fontDraw("GAME OVER");
		fontEnd();
	} else {
		fontBegin();
		fontPosition(88, 80);
		fontDraw("WORLD 1-1");
		fontPosition(120, 112);
		fontDraw("x");
		fontPosition(144, 112);
		fontDraw("%d", g_player.m_left);
		fontEnd();
		g_player.draw(vec2(96, 105));
	}
	*/
	fontBegin();
	fontPosition(88, 80);
	fontDraw("WORLD 1-1");
	fontPosition(120, 112);
	fontDraw("x");
	fontPosition(144, 112);
	fontDraw("%d", g_player.m_left);
	fontEnd();
	//g_player.draw(vec2(96, 105));
}