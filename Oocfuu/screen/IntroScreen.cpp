#include "IntroScreen.h"

#include "App.h"
#include "Game.h"
#include "common/font.h"
#include "Rect.h"
#include "TextureManager.h"
#include "input/Keyboard.h"
#include "Player/Player.h"
#include "world/CourseLoader.h"
#include "world/GimmickPart.h"
#include "world/CourseManager.h"

#include <Windows.h>

using namespace glm;

void IntroScreen::init()
{
	m_counter = 0;
	m_intro = INTRO_LOAD;
}

void IntroScreen::reset()
{
	g_courseManager.setScroll(0.0f);
	m_pGame->m_timer.setVisible(false);
	m_counter = 0;
	glClearColor(0, 0, 0, 1);

	if (m_pGame->m_isGameOver)
		m_intro = INTRO_GAMEOVER;
	else if (m_pGame->m_timer.isEnded())
		m_intro = INTRO_TIMEUP;
	else
		m_intro = INTRO_LOAD;
}

void IntroScreen::update()
{
	if (++m_counter > 60 * 2) {
		switch (m_intro) {
		case INTRO_LOAD:
		{
			Course course;

			// コースのファイル名を設定
			char filePath[64];
 			sprintf_s(filePath, sizeof filePath, "resource\\course\\course%d-%d.xml", 
						m_pGame->m_world.world, m_pGame->m_world.stage);
			//sprintf_s(filePath, sizeof filePath, "resource\\course\\course%d-%d.xml", 2, 4);

			// コースローダーを取得し、コースを初期化
			CourseLoader* pLoader = CourseLoader::create(&g_courseManager);
			pLoader->initialize(filePath);

			// コースを読み込む
			if (pLoader->load(&course)) {
				// コースの読み込みに成功
				g_courseManager.setScroll(0.0f);
				g_player.respawn((float)g_courseManager.getStartPosition().x, (float)g_courseManager.getStartPosition().y);
				m_pGame->setScreen(GAME_SCREEN_MAIN);
			} else {
				// コースの読み込みに失敗
				CourseLoader::Result error = pLoader->getLastError();
			}

			break;
		}
		case INTRO_GAMEOVER:
			m_intro = INTRO_LOAD;
			m_pGame->updateTopScore();
			m_pGame->reset();
			m_pGame->setScreen(GAME_SCREEN_TITLE);
			break;
		case INTRO_TIMEUP:
			break;
		}
	}
}

void IntroScreen::draw()
{
	m_pGame->drawHUD();

	switch (m_intro) {
	case INTRO_LOAD:
		g_textureManager.setTexture(TEXTURE_PLAYER_IDLE);
		Rect(vec2(PLAYER_SIZE, PLAYER_SIZE), vec2(96, 105)).draw();
		g_textureManager.unbindTexture();

		fontBegin();
		fontPosition(88, 80);
		fontDraw("WORLD %d-%d", m_pGame->m_world.world, m_pGame->m_world.stage);
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