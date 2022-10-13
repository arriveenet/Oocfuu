#include "MainScreen.h"
#include "../Header.h"
#include "../FrameCounter.h"

void MainScreen::init()
{
	m_debugDisplay = false;
}

void MainScreen::reset()
{
	/*
	alSourcei(
		g_sound[SOUND_BGM_GROUND].m_sid,	// ALuint sid
		AL_LOOPING,							// ALenum param
		AL_TRUE);							//ALint value

	alSourcef(
		g_sound[SOUND_BGM_GROUND].m_sid,		// ALuint sid
		AL_GAIN,	// ALenum param
		.08f);		// ALfloat value

	audioPlay(g_sound[SOUND_BGM_GROUND].m_sid);
	*/
	glClearColor({ 92 / 255.f }, { 148 / 255.f }, { 252 / 255.f }, { 1 });
	g_course.reload("resource\\course\\course1-1.txt", COURSE_WIDTH, COURSE_HEIGHT);
}

void MainScreen::update()
{
	if (Keyboard::m_nowPressed[0x1b])
		g_game.setScreen(GAME_SCREEN_TITLE);

	if (Keyboard::m_nowPressed[KEYBOARD_KEY_F3])
		m_debugDisplay = m_debugDisplay ? false : true;

	g_course.update();
	g_player.update();
}

void MainScreen::draw()
{
	g_game.drawHUD();
	g_course.draw();
	g_player.draw();

	if (m_debugDisplay) {
		glColor3ub(0x00, 0xff, 0x00);
		fontBegin();
		fontPosition(0, 0);
		fontDraw("FPS: %d", g_frameCounter.getFrameCount());
		fontPosition(0, 8 * 4);
		fontDraw("POSITION:%f,%f\n", g_player.m_position.x, g_player.m_position.y);
		fontDraw("SPEED   :%f,%f\n", g_player.m_speed.x, g_player.m_speed.y);
		fontDraw("JUMPING :%d\n", g_player.m_jumping);
		fontDraw("JUMPING_COUNT:%d\n", g_player.m_jumpCount);
		fontEnd();
	}
}