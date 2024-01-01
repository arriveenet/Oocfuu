#pragma once
#include "HbdScreen.h"
#include "score/Voyager.h"
#include "sound/Music.h"
#include "common/font.h"
#include "Game.h"
#include "input/Keyboard.h"
#include "TextureManager.h"
#include "Player/Player.h"
#include "world/CourseManager.h"

#include <freeglut.h> 

using namespace glm;

HbdScreen::HbdScreen()
{
}

void HbdScreen::init()
{
	g_music.setMusic(MusicType::HappyBirthday);
}

void HbdScreen::reset()
{
	g_music.setMusic(MusicType::HappyBirthday);
	glClearColor({ 0 }, { 0 }, { 0 }, { 1 });
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
	
	if (g_music.m_end) {
		g_music.reset();
		g_music.setMusic(MusicType::TyphoonParade);
		g_music.play();

		g_game.setScreen(GAME_SCREEN_TP);
	}
	
}

void HbdScreen::draw()
{
	g_courseManager.draw();
	g_game.drawHUD();

	fontBegin();
	fontPosition(72, 128);
	fontDraw("HAPPY BIRTHDAY");
	fontEnd();
	// Draw cake
	g_textureManager.setTexture(TEXTURE_CAKE);
	Rect(vec2(32, 32), vec2(16 * 7, 16 * 11)).draw();
	g_textureManager.unbindTexture();

	g_player.draw();
}