#include <gl/glut.h>

#include "MainScreen.h"
#include "App.h"
#include "Game.h"
#include "common/font.h"
#include "common/FrameCounter.h"
#include "world/Part.h"
#include "world/CourseManager.h"
#include "world/GimmickPart.h"
#include "Player/Player.h"
#include "input/Keyboard.h"
#include "sound/Sound.h"

void MainScreen::init()
{
}

void MainScreen::reset()
{
	COLORREF clearColor = g_courseManager.getClearColor();
	glClearColor(GetRValue(clearColor) / 255.0f, GetGValue(clearColor) / 255.0f, GetBValue(clearColor) / 255.0f, 1.0f);
	g_game.m_timer.start();
	g_game.m_timer.setVisible(true);
}

void MainScreen::update()
{
	if (Keyboard::m_nowPressed[0x1b])
		g_game.setScreen(GAME_SCREEN_TITLE);

	if (Keyboard::m_nowPressed[KEYBOARD_KEY_F3])
		Game::m_debugInfo = Game::m_debugInfo ? false : true;

	if (Keyboard::m_nowPressed['p'] && !g_player.m_goal)
		g_game.pause();

	if (g_game.isPause())
		return;

	g_courseManager.update();
	g_gmmickPart.update();
	g_enemyManager.update();
	g_player.update();

	if ((g_player.m_speed.x > 0)
		&& (g_player.m_position.x > g_courseManager.getScroll() + SCREEN_WIDTH / 2 - PLAYER_SIZE / 2)) {
		g_courseManager.addScroll(g_player.m_speed.x);
	}
	g_courseManager.setScroll(std::min(g_courseManager.getScroll(), (float)(g_courseManager.getWidth() * PART_SIZE - SCREEN_WIDTH)));
}

void MainScreen::draw()
{
	g_game.drawHUD();
	g_courseManager.draw();
	g_gmmickPart.draw();
	g_enemyManager.draw();
	g_player.draw();


	if (Game::m_debugInfo) {
		fontBegin();
		fontColor(0x00, 0xff, 0x00);
		fontBackgroundColor(true);
		fontPosition(0, 0);
		fontDraw("FPS:%d\n", g_frameCounter.getFrameCount());
		fontDraw("SCROLL:%f\n", g_courseManager.getScroll());

		fontPosition(0, 8 * 4);
		fontDraw("POSITION:%f,%f\n", g_player.m_position.x, g_player.m_position.y);
		fontDraw("SPEED   :%f,%f\n", g_player.m_speed.x, g_player.m_speed.y);
		//fontDraw("STATE   :%s\n", m_pStateContext->getString().c_str());
		fontDraw("ANIMATION:%d  JUMPING:%d\n", g_player.m_animationController.m_animation, g_player.m_jumping);
		fontDraw("FALLING :%d  DEAD:%d\n", g_player.m_falling, g_player.m_dead);
		fontDraw("ENEMYCOUNT:%d\n", g_enemyManager.getEnemySize());
		fontBackgroundColor(false);
		fontColor(0xff, 0xff, 0xff);
		fontEnd();
	}
}