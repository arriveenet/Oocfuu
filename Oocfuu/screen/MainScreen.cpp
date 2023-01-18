#include <gl/glut.h>

#include "MainScreen.h"
#include "App.h"
#include "Game.h"
#include "font.h"
#include "Part.h"
#include "Course.h"
#include "Player/Player.h"
#include "FrameCounter.h"
#include "input/Keyboard.h"
#include "world/GimmickPart.h"
#include "sound/Sound.h"

void MainScreen::init()
{
	m_pause = false;
}

void MainScreen::reset()
{
	m_pause = false;
	COLORREF clearColor = g_courseManager.getClearColor();
	glClearColor(GetRValue(clearColor) / 255.0f, GetGValue(clearColor) / 255.0f, GetBValue(clearColor) / 255.0f, 1.0f);
	g_game.resetTimer();
	g_game.startTimer();
	g_game.visibleTimer(true);
}

void MainScreen::update()
{
	if (Keyboard::m_nowPressed[0x1b])
		g_game.setScreen(GAME_SCREEN_TITLE);

	if (Keyboard::m_nowPressed[KEYBOARD_KEY_F3])
		Game::m_debugInfo = Game::m_debugInfo ? false : true;

	g_courseManager.update();
	g_gmmickPart.update();
	g_player.update();
	g_enemyManager.update();

	if ((g_player.m_speed.x > 0)
		&& (g_player.m_position.x > g_courseManager.m_scroll + GLUT_SCREEN_WIDTH / 2 - PLAYER_SIZE / 2)) {
		g_courseManager.m_scroll += g_player.m_speed.x;
	}
	g_courseManager.m_scroll = std::min(g_courseManager.m_scroll, (float)(g_courseManager.getWidth() * PART_SIZE - SCREEN_WIDTH));
}

void MainScreen::draw()
{
	g_game.drawHUD();
	g_courseManager.draw();
	g_gmmickPart.draw();
	g_player.draw();
	g_enemyManager.draw();

	if (Game::m_debugInfo) {
		fontBegin();
		fontColor(0x00, 0xff, 0x00);
		fontBackgroundColor(true);
		fontPosition(0, 0);
		fontDraw("FPS:%d", g_frameCounter.getFrameCount());
		fontBackgroundColor(false);
		fontColor(0xff, 0xff, 0xff);
		fontEnd();
	}
}