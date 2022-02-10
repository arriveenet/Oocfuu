#include "MainScreen.h"
#include "../Header.h"

void MainScreen::init()
{
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

	g_player.update();
	/*
	g_playerDie.update();
	g_playerGoal.update();
	Kuribo::updateAll();
	g_Nokonoko.update();
	*/
}

void MainScreen::draw()
{
	g_game.drawHUD();
	g_course.draw();
	g_player.draw();
	/*
	g_playerDie.draw();
	g_playerGoal.draw();
	Kuribo::drawAll();
	g_Nokonoko.draw();

	fontBegin();
	{
		fontPosition(0, 0);
		fontDraw("Posision:%.1f, %.1f\n", g_player.m_position.x, g_player.m_position.y);
	}
	fontEnd();
	*/
}