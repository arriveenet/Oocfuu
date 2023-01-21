#include "App.h"

#include "font.h"
#include "Part.h"
#include "Course.h"
#include "Game.h"
#include "FrameCounter.h"
#include "TextureManager.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "sound\audio.h"
#include "sound\Sound.h"
#include "sound\Music.h"
#include "sound\Channel.h"
#include "animation\Animation.h"
#include "animation\AnimationController.h"
#include "Player/Player.h"

#include <stdio.h>
#include <windows.h>
#include <stdio.h>
#include <gl/freeglut.h>

App g_app;
extern glm::ivec2 windowSize;

App::App()
	: m_running(false)
{
	// コンソールハンドルを取得
	m_hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	time_t t = time(NULL);
	localtime_s(&m_currentTime, &t);
}

App::~App()
{
}

bool App::init()
{
	g_pSound = Sound::getInstance();

	if (!printInit(fontInit(), "Font init"))
		return false;

	if (!printInit(g_pSound->init(), "Sound init"))
		return false;

	if (!printInit(audioInit(), "Audio init"))
		return false;

	if (!printInit(g_game.init(), "Game init"))
		return false;

	if (!printInit(g_parts->initAll(), "Part initAll"))
		return false;

	if (!printInit(g_textureManager.init(), "Texture initAll"))
		return false;

	if (!printInit(g_animations->initAll(), "Animation init"))
		return false;

	if (!printInit(g_music.init(), "Music init"))
		return false;

	return true;
}

void App::release()
{
	g_player.~Player();
	fontRelease();
	audioRelease();
	g_courseManager.release();
	g_textureManager.release();
	g_pSound->release();
	g_game.release();
	Mouse::release();
}

void App::update()
{
	time_t t = time(NULL);
	localtime_s(&m_currentTime, &t);

	g_frameCounter.update();
//	g_courseManager.update();
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
		g_courseManager.m_scroll, g_courseManager.m_scroll + SCREEN_WIDTH,
		SCREEN_HEIGHT, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);//GLenum cap
	glEnable(GL_BLEND);
	glBlendFunc(
		GL_SRC_ALPHA,			//GLenum sfactor
		GL_ONE_MINUS_SRC_ALPHA);//GLenum dfactor

	{
		float f = (float)windowSize.x / SCREEN_WIDTH;
	//	glLineWidth(f);// GLfloat width
		glPointSize(f);// GLfloat size
	}

	glColor3ub(0xff, 0xff, 0xff);

	g_game.m_pCurrentScreen->draw();
}

void App::run()
{
	m_running = true;

	while (m_running) {
		//update();
		//draw();
		glutMainLoopEvent();
	}
}

bool App::printInit(int _result, const char* _str)
{
	return printInit(_result == 0 ? true : false, _str);
}

bool App::printInit(bool _result, const char* _str)
{
	printf("[");
	if (_result) {
		SetConsoleTextAttribute(
			m_hConsoleOutput,	// HANDLE hConsoleOutput,
			FOREGROUND_GREEN);	// WORD wAttributes
			printf("  OK  ");
	} else {
		SetConsoleTextAttribute(
			m_hConsoleOutput,	// HANDLE hConsoleOutput,
			FOREGROUND_RED);	// WORD wAttributes
		printf("FAILED");
	}
	SetConsoleTextAttribute(
		m_hConsoleOutput,										// HANDLE hConsoleOutput,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);	// WORD wAttributes
	printf("] %s\n", _str);

	return _result;
}
