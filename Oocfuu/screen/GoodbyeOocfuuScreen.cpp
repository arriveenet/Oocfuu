/*
* Ç≥ÇÊÇ§Ç»ÇÁÅAÇ®Ç≠Ç”
*/

#include "GoodbyeOocfuuScreen.h"
#include "input/Keyboard.h"
#include "sound/Music.h"
#include "Game.h"
#include "common/font.h"
#include "Texture.h"
#include "player/Player.h"
#include "world/CourseManager.h"
#include "world/CourseLoader.h"
#include "score/Voyager.h"

#include <GL/freeglut.h>

void GoodbyeOocfuuScreen::init()
{

}

void GoodbyeOocfuuScreen::reset()
{
	g_music.reset();
	g_music.resetScore();

	g_music.setMusic(MusicType::Voyager);

	g_music.play();

	Course course;
	CourseLoader* pLoader = CourseLoader::create(&g_courseManager);
	if (pLoader->initialize("resource\\course\\oocfuu.xml")) {
		pLoader->load(&course);
	}
}

void GoodbyeOocfuuScreen::update()
{
	g_courseManager.update();

	if (Keyboard::m_nowPressed['r']) {
		g_music.reset();
		g_music.play();
	}
	if (Keyboard::m_nowPressed['k']) {
		g_music.m_play ? g_music.stop() : g_music.play();
	}

	if (Keyboard::m_nowPressed[0x1b])
		m_pGame->setScreen(GAME_SCREEN_TITLE);
}

void GoodbyeOocfuuScreen::draw()
{
	g_courseManager.draw();

	fontBegin();
	fontPosition(8*9, 8*12);
	glColor3ub(0xfe, 0xfe, 0xfe);
	fontDraw("GOODBYE OOCFUU.");
	fontEnd();

}