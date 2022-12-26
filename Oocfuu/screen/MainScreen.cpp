#include <gl/glut.h>

#include "MainScreen.h"
#include "../App.h"
#include "../Game.h"
#include "input/Keyboard.h"
#include "../font.h"
#include "../Player/Player.h"
#include "../Part.h"
#include "../Course.h"
#include "../FrameCounter.h"

void MainScreen::init()
{
	m_debugDisplay = true;
}

void MainScreen::reset()
{
	glClearColor({ 92 / 255.f }, { 148 / 255.f }, { 252 / 255.f }, { 1 });
	g_courseManager.reload("resource\\course\\course1-1.txt", COURSE_WIDTH, COURSE_HEIGHT);
}

void MainScreen::update()
{
	if (Keyboard::m_nowPressed[0x1b])
		g_game.setScreen(GAME_SCREEN_TITLE);

	if (Keyboard::m_nowPressed[KEYBOARD_KEY_F3])
		m_debugDisplay = m_debugDisplay ? false : true;

	g_courseManager.update();
	g_player.update();
	m_enemyManager.update();

	if ((g_player.m_speed.x > 0)
		&& (g_player.m_position.x > g_courseManager.m_scroll + GLUT_SCREEN_WIDTH / 2 - PLAYER_SIZE / 2)) {
		g_courseManager.m_scroll += g_player.m_speed.x;
	}
	g_courseManager.m_scroll = std::min(g_courseManager.m_scroll, (float)(COURSE_WIDTH * PART_SIZE - SCREEN_WIDTH));
}

void MainScreen::draw()
{
	g_game.drawHUD();
	g_courseManager.draw();
	g_player.draw();
	m_enemyManager.draw();

	if (m_debugDisplay) {
		//glColor3ub(0x00, 0xff, 0x00);
		//fontBegin();
		//fontPosition(0, 0);
		//fontDraw("FPS: %d", g_frameCounter.getFrameCount());
		//fontPosition(0, 8 * 4);
		//fontDraw("POSITION:%f,%f\n", g_player.m_position.x, g_player.m_position.y);
		//fontDraw("SPEED   :%f,%f\n", g_player.m_speed.x, g_player.m_speed.y);
		//fontDraw("JUMPING :%d\n", g_player.m_jumping);
		//fontDraw("JUMPING_COUNT:%d\n", g_player.m_jumpCount);
		//fontEnd();
	}
}