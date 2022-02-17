#pragma once
#include "HbdScreen.h"
#include "../HappyBirthdayToYou.h"
#include "../TyphoonParade.h"
#include "../Voyager.h"
#include "../Music.h"
#include "../font.h"
#include "../Game.h"
#include "../Keyboard.h"
#include "../Texture.h"
#include "../Player.h"
#include "../Course.h"

#include <freeglut.h> 

HbdScreen::HbdScreen()
{
}

void HbdScreen::init()
{
	g_music.setScore(AUDIO_CHANNEL_PULSE0, hbtyPulse0, HBTY_PULSE0_COUNT);
	g_music.setScore(AUDIO_CHANNEL_PULSE1, hbtyPulse1, HBTY_PULSE1_COUNT);
	g_music.setScore(AUDIO_CHANNEL_TRIANGLE, hbtyTri, HBTY_TRI_COUNT);
}

void HbdScreen::reset()
{
	g_music.resetScore();
	g_music.setScore(AUDIO_CHANNEL_PULSE0, hbtyPulse0, HBTY_PULSE0_COUNT);
	g_music.setScore(AUDIO_CHANNEL_PULSE1, hbtyPulse1, HBTY_PULSE1_COUNT);
	g_music.setScore(AUDIO_CHANNEL_TRIANGLE, hbtyTri, HBTY_TRI_COUNT);
	glClearColor({ 0 }, { 0 }, { 0 }, { 1 });
	g_course.reload("resource\\course\\course.txt", 16, 15);
}

void HbdScreen::update()
{
	g_player.update();

	if (Keyboard::m_nowPressed[' '])
		g_music.play();

	if (Keyboard::m_nowPressed[0x1b]) {
		g_music.reset();
		g_game.setScreen(GAME_SCREEN_TITLE);
	}

	if (Keyboard::m_nowPressed['o']) {
		printf("Ç®Ç≠Ç”ÇøÇ·ÇÒÇ∆íáó«Ç≠Ç»ÇËÇΩÇ©Ç¡ÇΩÇ»ÇüÅB\n");
		g_music.reset();
		g_music.resetScore();
		g_music.setScore(AUDIO_CHANNEL_PULSE0, voyPulse0, VOY_PULSE0_COUNT);
		g_music.setScore(AUDIO_CHANNEL_PULSE1, voyPulse1, VOY_PULSE1_COUNT);
		g_music.setScore(AUDIO_CHANNEL_TRIANGLE, voyTri, VOY_TRI_COUNT);
		g_music.setScore(AUDIO_CHANNEL_NOISE, NULL, 0);
		g_music.play();
		g_game.setScreen(GAME_SCREEN_GO);
	}
	
	if (g_music.m_end) {
		g_music.reset();
		g_music.resetScore();
		g_music.setScore(AUDIO_CHANNEL_PULSE0, tpP0, TP_PULSE0_COUNT);
		g_music.setScore(AUDIO_CHANNEL_PULSE1, tpP1, TP_PULSE1_COUNT);
		g_music.setScore(AUDIO_CHANNEL_TRIANGLE, tpTri, TP_TRI_COUNT);
		g_music.setScore(AUDIO_CHANNEL_NOISE, tpNoise, TP_NOISE_COUNT);
		g_music.play();

		g_game.setScreen(GAME_SCREEN_TP);
	}
	
}

void HbdScreen::draw()
{
	g_course.draw();
	g_game.drawHUD();

	fontBegin();
	fontPosition(72, 128);
	fontDraw("HAPPY BIRTHDAY");
	fontEnd();
	// Draw cake
	glBindTexture(
		GL_TEXTURE_2D,
		g_textures[TEXTURE_CAKE].m_texture);
	Rect(vec2(32, 32), vec2(16 * 7, 16 * 11)).draw();

	g_player.draw();
}