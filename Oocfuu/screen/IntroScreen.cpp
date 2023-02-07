#include "IntroScreen.h"

#include "App.h"
#include "Game.h"
#include "common/font.h"
#include "world/Course.h"
#include "Rect.h"
#include "TextureManager.h"
#include "input/Keyboard.h"
#include "Player/Player.h"
#include "world/GimmickPart.h"

#include <Windows.h>

using namespace glm;

void IntroScreen::init()
{
	m_counter = 0;
	m_intro = INTRO_LOAD;
}

void IntroScreen::reset()
{
	g_courseManager.m_scroll = 0.0f;
	g_game.m_timer.setVisible(false);
	m_counter = 0;
	glClearColor(0, 0, 0, 1);

	if (g_game.m_isGameOver)
		m_intro = INTRO_GAMEOVER;
	else
		m_intro = INTRO_LOAD;
}

void IntroScreen::update()
{
	if (++m_counter > 60 * 2) {
		switch (m_intro) {
		case INTRO_LOAD:
			char filePath[64];
			sprintf_s(filePath, sizeof filePath, "resource\\course\\course%d-%d.txt", g_game.m_world.world, g_game.m_world.stage);
			// コースファイルを読み込む
			if (g_courseManager.load(filePath)) {
				g_courseManager.m_scroll = 0.0f;
				g_player.respawn((float)g_courseManager.getStartPosition().x, (float)g_courseManager.getStartPosition().y);
				g_game.setScreen(GAME_SCREEN_MAIN);
			} else {
			// Convert a basic_string string to a wide character
			// wchar_t* string. You must first convert to a char*
			// for this to work.
				const size_t newsizew = g_courseManager.getErrorString().size() + 1;
				size_t convertedChars = 0;
				wchar_t* wcstring = new wchar_t[newsizew];
				mbstowcs_s(&convertedChars, wcstring, newsizew, g_courseManager.getErrorString().c_str(), _TRUNCATE);
				MessageBox(
					NULL,				// HWND hWnd,
					(LPCWSTR)wcstring,	// LPCWSTR lpText,
					(LPCWSTR)L"Error",	// LPCWSTR lpCaption,
					MB_ICONHAND			// UINT uType
				);
				delete[] wcstring;
				g_app.release();
				exit(EXIT_FAILURE);
			}
			break;
		case INTRO_GAMEOVER:
			m_intro = INTRO_LOAD;
			g_game.updateTopScore();
			g_game.reset();
			g_game.setScreen(GAME_SCREEN_TITLE);
			break;
		case INTRO_TIMEUP:
			break;
		}
	}
}

void IntroScreen::draw()
{
	g_game.drawHUD();

	switch (m_intro) {
	case INTRO_LOAD:
		g_textureManager.setTexture(TEXTURE_PLAYER_IDLE);
		Rect(vec2(PLAYER_SIZE, PLAYER_SIZE), vec2(96, 105)).draw();
		g_textureManager.unbindTexture();

		fontBegin();
		fontPosition(88, 80);
		fontDraw("WORLD %d-%d", g_game.m_world.world, g_game.m_world.stage);
		fontPosition(120, 112);
		fontDraw("x");
		fontPosition(144, 112);
		fontDraw("%d", g_player.m_left);
		fontEnd();

		break;
	case INTRO_GAMEOVER:
		fontBegin();
		fontPosition(88, 112);
		fontDraw("GAME OVER");
		fontEnd();
		break;
	case INTRO_TIMEUP:
		fontBegin();
		fontPosition(88, 80);
		fontDraw("TIME UP");
		fontEnd();
		break;
	}

}

/**
* stringからwstringへの変換参考サイト
* https://learn.microsoft.com/ja-jp/cpp/text/how-to-convert-between-various-string-types?view=msvc-170#converting-between-narrow-and-wide-strings
*/