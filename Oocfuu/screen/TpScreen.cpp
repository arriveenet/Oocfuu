#include "TpScreen.h"
#include "sound/Music.h"
#include "common/font.h"
#include "input/Keyboard.h"
#include "Game.h"
//#include "../Firework.h"
#include "TextureManager.h"
#include "player/Player.h"
#include "world/CourseManager.h"
#include "common/FrameCounter.h"

#include <freeglut.h>

#define SCREEN_WIDTH	256
#define SCREEN_HEIGHT	240
#define FIREWORK_COUNT_MAX	3

using namespace glm;

static int count = FIREWORK_COUNT_MAX;
static const vec2 fireworkPosition[FIREWORK_COUNT_MAX] = {
	{vec2(16*8,16*10.5)},
	{vec2(16*13,16*8)},
	{vec2(16*5,16*10.5)},
};

static const MESSAGE messages[] =
{
	{"HAPPY BIRTHDAY OOCFUU!",		vec2(40, 64),	96},
	{"FEBRUARY,4,2022",				vec2(120, 88),	192},
	{"THANK YOU FOR A FUN",			vec2(56,104),	288},
	{"LIVESTREAMING ALWAYS.",		vec2(48, 120),	288},
	{"HOPE YOU HAVE A GREAT YEAR!",	vec2(24, 136),	384},
	{"FROM OSHU-FUJIWARA",			vec2(96, 160),	480}
};

#define MESSAGE_MAX (sizeof(messages) / sizeof(MESSAGE))

void TpScreen::init()
{
	m_messageController.setMessage(messages, MESSAGE_MAX);
}

void TpScreen::reset()
{
	m_messageController.start();
}

void TpScreen::update()
{
	//g_firework.update();
	g_player.update();

	if (g_music.m_play)
		m_messageController.update();

	//if (g_music.m_end) {
	//	if (
	//		(!g_firework.m_explosion)
	//		&& (count > 0)
	//		) {
	//		g_firework.start(fireworkPosition[count-1]);
	//		count--;
	//	}
	//}
	if (Keyboard::m_nowPressed['r']) {
		g_music.reset();
		g_music.play();
		m_messageController.start();
		count = FIREWORK_COUNT_MAX;
	}
	if (Keyboard::m_nowPressed['k']) {
		g_music.m_play ? g_music.stop() : g_music.play();
	}

	if (Keyboard::m_nowPressed[0x1b]) {
		g_music.reset();
		m_pGame->setScreen(GAME_SCREEN_TITLE);
	}
}

void TpScreen::draw()
{
	g_courseManager.draw();
	m_pGame->drawHUD();
	//g_firework.draw();

	// Draw cake
	g_textureManager.setTexture(TEXTURE_CAKE);
	Rect(vec2(32, 32), vec2(16 * 7, 16 * 11)).draw();

	m_messageController.draw();

	g_player.draw();
}