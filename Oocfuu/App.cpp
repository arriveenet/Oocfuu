#include "App.h"
#include "common/font.h"
#include "world/Part.h"
#include "world/Course.h"
#include "Game.h"
#include "common/FrameCounter.h"
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

#include "world/CourseLoader.h"

#include <stdio.h>
#include <windows.h>
#include <chrono>
#include <gl/freeglut.h>

#define MS_PER_UPDATE	(1000 / 30)

App g_app;
extern glm::ivec2 windowSize;

App::App()
	: m_running(false)
	, m_hConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE))
	, m_currentTime()
{
}

App::~App()
{
}

bool App::init()
{
	time_t t = time(NULL);
	localtime_s(&m_currentTime, &t);

	srand((unsigned int)time(NULL));

	g_pSound = Sound::getInstance();

	if (!printInit(fontInit(), "Font init"))
		return false;

	if (!printInit(g_pSound->init(), "Sound init"))
		return false;

	if (!printInit(audioInit(), "Audio init"))
		return false;

	if (!printInit(g_game.init(), "Game init"))
		return false;

	if (!printInit(g_textureManager.init(), "Texture initAll"))
		return false;

	if (!printInit(g_partManager.init(), "Part init"))
		return false;

	if (!printInit(g_animations->initAll(), "Animation init"))
		return false;

	if (!printInit(g_music.init(), "Music init"))
		return false;

	CourseLoader* pLoader = CourseLoader::create();
	pLoader->initialize("book.xml");

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
		g_courseManager.getScroll(), g_courseManager.getScroll() + SCREEN_WIDTH,
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

	std::chrono::system_clock::time_point previous = std::chrono::system_clock::now();
	double lag = 0.0;

	while (true) {
		std::chrono::system_clock::time_point current = std::chrono::system_clock::now();
		std::chrono::milliseconds millisec = std::chrono::duration_cast<std::chrono::milliseconds>(current - previous);
		//double elapsed = millisec.count();
		double elapsed = 0.0;
		previous = current;
		lag += elapsed;

		glutMainLoopEvent();

		while (lag >= MS_PER_UPDATE) {
			update();
			lag -= MS_PER_UPDATE;
		}

		draw();
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
