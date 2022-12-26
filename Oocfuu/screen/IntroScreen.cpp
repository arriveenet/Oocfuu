#include "IntroScreen.h"
#include "../Game.h"
#include "input/Keyboard.h"
#include "../font.h"
#include "../Player/Player.h"
#include "../Course.h"
#include "../Rect.h"
#include "../TextureManager.h"

using namespace glm;

void IntroScreen::init()
{
	m_counter = 0;
	m_intro = INTRO_LOAD;
}

void IntroScreen::reset()
{
	m_counter = 0;
	glClearColor({ 0 }, { 0 }, { 0 }, { 1 });

	if (g_game.m_isGameOver)
		m_intro = INTRO_GAMEOVER;
	else
		m_intro = INTRO_LOAD;
}

void IntroScreen::update()
{
	if (++m_counter > 60 * 3) {
		switch (m_intro) {
		case INTRO_LOAD:
			g_player.respawn(PLAYER_DEFAULT_X, PLAYER_DEFAULT_Y);
			g_courseManager.m_scroll = 0.0f;
			g_game.setScreen(GAME_SCREEN_MAIN);
			break;
		case INTRO_GAMEOVER:
			m_intro = INTRO_LOAD;
			g_game.m_isGameOver = false;
			g_game.setScreen(GAME_SCREEN_TITLE);
			break;
		case INTRO_TIMEUP:
			break;
		}
	}
}

void IntroScreen::draw()
{
	g_game.drawHUD();

	switch (m_intro) {
	case INTRO_LOAD:
		g_textureManager.setTexture(TEXTURE_PLAYER_IDLE);
		Rect(vec2(PLAYER_SIZE, PLAYER_SIZE), vec2(96, 105)).draw();
		g_textureManager.unbindTexture();

		fontBegin();
		fontPosition(88, 80);
		fontDraw("WORLD 1-1");
		fontPosition(120, 112);
		fontDraw("x");
		fontPosition(144, 112);
		fontDraw("%d", g_player.m_left);
		fontEnd();

		break;
	case INTRO_GAMEOVER:
		fontBegin();
		fontPosition(88, 112);
		fontDraw("GAME OVER");
		fontEnd();
		break;
	case INTRO_TIMEUP:
		fontBegin();
		fontPosition(88, 80);
		fontDraw("TIME UP");
		fontEnd();
		break;
	}

}