#include "App.h"

#include "font.h"
#include "tex.h"
#include "sound\audio.h"
#include "Keyboard.h"
#include "Rect.h"
#include "Part.h"
#include "Course.h"
#include "Texture.h"
#include "sound\Sound.h"
#include "sound\Music.h"
#include "sound\Channel.h"
#include "Game.h"
#include "animation\Animation.h"
#include "animation\AnimationController.h"
#include "Player.h"
#include "Firework.h"
#include "FrameCounter.h"

#include <stdio.h>
#include <gl/glut.h>

App g_app;

bool printInit(int result, const char* _str)
{
	if (result == 0) {
		printf("[  OK  ] %s\n", _str);
		return true;
	} else {
		printf("[FAILED] %s\n", _str);
		return false;
	}
}

App::App()
{
	time_t t = time(NULL);
	localtime_s(&m_currentTime, &t);
}

App::~App()
{
	fontRelease();
	g_game.release();
}

bool App::init()
{
	if (!printInit(fontInit(), "Font init"))
		return false;

	if (!printInit(audioInit(), "Audio init"))
		return false;

	if (!printInit(g_game.init(), "Game init"))
		return false;

	if (!printInit(g_parts->initAll(), "Part initAll"))
		return false;

	if (!printInit(g_textures->initAll(), "Texture initAll"))
		return false;

	if (!printInit(g_player.init(), "Player init"))
		return false;

	if (!printInit(g_firework.init(), "Firework init"))
		return false;

	if (!printInit(g_animations->initAll(), "Animation init"))
		return false;

	if (!printInit(g_sound->initAll(), "Sound init"))
		return false;

	if (!printInit(g_music.init(), "Music init"))
		return false;

	//g_course.load("resource\\course\\course1-1.txt");


	return true;
}

void App::update()
{
	time_t t = time(NULL);
	localtime_s(&m_currentTime, &t);

	g_frameCounter.update();
	g_music.update();
	g_game.update();
	g_game.m_pCurrentScreen->update();
}

void App::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(
		g_course.m_scroll, g_course.m_scroll + SCREEN_WIDTH,
		SCREEN_HEIGHT, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);//GLenum cap
	glEnable(GL_BLEND);
	glBlendFunc(
		GL_SRC_ALPHA,			//GLenum sfactor
		GL_ONE_MINUS_SRC_ALPHA);//GLenum dfactor

	glColor3ub(0xff, 0xff, 0xff);

	g_game.m_pCurrentScreen->draw();

	/*fontBegin();
	{
		glColor3ub(0x00, 0xff, 0x00);
		fontPosition(0, 0);
		fontDraw("FPS:%d", g_frameCounter.getFrameCount());
	}
	fontEnd();*/
}