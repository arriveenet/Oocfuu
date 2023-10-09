#include <Windows.h>
#include <gl/freeglut.h>

#include "TitleScreen.h"
#include "App.h"
#include "Game.h"
#include "input/Keyboard.h"
#include "common/font.h"
#include "Player/Player.h"
#include "world/CourseLoader.h"
#include "world/CourseManager.h"
#include "TextureManager.h"

using namespace glm;

enum {
	MODE_1P,
	MODE_HBD,
	MODE_MAX
};

static const char* modeNames[] = {
	"1 PLAYER GAME",
	"HAPPY BIRTHDAY"
};

TitleScreen::TitleScreen()
	: m_mode(MODE_1P)
	, m_command()
{
	glClearColor({ 80 / 255.f }, { 128 / 255.f }, { 255 / 255.f }, { 1 });
}

void TitleScreen::init()
{
	Course course;

	// コースを読み込む
	CourseLoader* pLoader = CourseLoader::create(&g_courseManager);
	pLoader->initialize("resource\\course\\course1-1.xml");
	pLoader->load(&course);

	g_player.respawn((float)g_courseManager.getStartPosition().x, (float)g_courseManager.getStartPosition().y);
}

void TitleScreen::reset()
{
	m_command.clear();
	g_game.m_timer.setVisible(false);

	Course course;

	// コースを読み込む
	CourseLoader* pLoader = CourseLoader::create(&g_courseManager);
	pLoader->initialize("resource\\course\\course1-1.xml");
	pLoader->load(&course);

	COLORREF clearColor = g_courseManager.getClearColor();
	glClearColor(GetRValue(clearColor) / 255.0f, GetGValue(clearColor) / 255.0f, GetBValue(clearColor) / 255.0f, 1.0f);
	g_courseManager.setScroll(0.0f);
	g_player.reset();
	g_player.respawn((float)g_courseManager.getStartPosition().x, (float)g_courseManager.getStartPosition().y);
}
void TitleScreen::update()
{
	g_courseManager.update();
	
	if (Keyboard::m_nowPressed[KEYBOARD_KEY_LEFT]) {
		m_command.push_back('l');
	}
	if (Keyboard::m_nowPressed[KEYBOARD_KEY_UP]) {
		m_command.push_back('u');
		m_mode--;
	}
	if (Keyboard::m_nowPressed[KEYBOARD_KEY_RIGHT]) {
		m_command.push_back('r');
	}
	if (Keyboard::m_nowPressed[KEYBOARD_KEY_DOWN]) {
		m_command.push_back('d');
		m_mode++;
	}
	if (Keyboard::m_nowPressed['b']) {
		m_command.push_back('b');
	}
	if (Keyboard::m_nowPressed['a']) {
		m_command.push_back('a');
	}

	if (Keyboard::m_nowPressed['w']) {
		m_command.push_back('w');
		m_mode--;
	}
	if (Keyboard::m_nowPressed['s']) {
		m_command.push_back('s');
		m_mode++;
	}

	m_mode = (MODE_MAX + m_mode) % MODE_MAX;

	if (Keyboard::m_nowPressed[0x0d] || Keyboard::m_nowPressed['f']) {
		if (m_command == "uuddlrlrba") {
			g_game.setScreen(GAME_SCREEN_GO);
		} else {
			switch (m_mode) {
			case MODE_1P:
				g_game.setScreen(GAME_SCREEN_INTRO);
				break;
			case MODE_HBD:
				g_game.setScreen(GAME_SCREEN_HBD);
				break;
			}
		}
	}

	if (Keyboard::m_nowPressed[0x1b]) {
		g_app.close();
	}

}

void TitleScreen::draw()
{
	g_courseManager.draw();
	g_game.drawHUD();

	g_textureManager.setTexture(TEXTURE_TITLE);
	Rect rect(vec2(174, 88), vec2(40,32));
	rect.draw();
	g_textureManager.unbindTexture();

	g_player.draw();

	fontBegin();
	fontPosition(120, 120);
	fontColor(252, 188, 176);
	fontDraw("b2022 OOCFUU");
	fontColor(0xff, 0xff, 0xff);
	fontPosition(72, 144);
	for (int i = 0; i < MODE_MAX; i++) {
		fontDraw("%s %s\n\n", (i == m_mode) ? "a" : " ", modeNames[i]);
	}
	fontPosition(96, 184);
	fontDraw("TOP-%07d", g_game.getTopScore());
	fontEnd();
}