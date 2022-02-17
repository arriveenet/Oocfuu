#include "TpScreen.h"
#include "../Music.h"
#include "../font.h"
#include "../Keyboard.h"
#include "../Game.h"
#include "../Firework.h"
#include "../Texture.h"
#include "../Player.h"
#include "../Course.h"

#include <freeglut.h>

#define SCREEN_WIDTH	256
#define SCREEN_HEIGHT	240
#define FIREWORK_COUNT_MAX	3

static int count = FIREWORK_COUNT_MAX;
static const vec2 fireworkPosition[FIREWORK_COUNT_MAX] = {
	{vec2(16*8,16*10.5)},
	{vec2(16*13,16*8)},
	{vec2(16*5,16*10.5)},
};

typedef struct {
	char str[32];
	vec2 position;
	unsigned int startCount;
}MESSAGE;

static MESSAGE messages[] = {
	{"HAPPY BIRTHDAY OOCFUU!",vec2(40, 64)},
	{"FEBRUARY,4,2022",vec2(120, 88)},
	{"THANK YOU FOR A FUN",vec2(56,104)},
	{"LIVESTREAMING ALWAYS.",vec2(48, 120)},
	{"HOPE YOU HAVE A GREAT YEAR!",vec2(24, 136)},
	{"FROM OSHU-FUJIWARA",vec2(96, 160)}
};

#define MESSAGE_MAX (sizeof(messages) / sizeof(MESSAGE))

void TpScreen::init()
{
	m_count = 0;
	int total = 96;
	for (int i = 0; i < MESSAGE_MAX; i++) {
		messages[i].startCount = total;
		if (i == 2)
			continue;
		total += 96;
	}
}

void TpScreen::reset()
{
	m_count = 0;
}

void TpScreen::update()
{
	g_firework.update();
	g_player.update();

	if (g_music.m_play)
		m_count++;

	if (g_music.m_end) {
		if (
			(!g_firework.m_explosion)
			&& (count > 0)
			) {
			g_firework.start(fireworkPosition[count-1]);
			count--;
		}
	}
	if (Keyboard::m_nowPressed['r']) {
		g_music.reset();
		g_music.play();
		m_count = 0;
		count = FIREWORK_COUNT_MAX;
	}
	if (Keyboard::m_nowPressed['k']) {
		g_music.m_play ? g_music.stop() : g_music.play();
	}

	if (Keyboard::m_nowPressed[0x1b]) {
		g_music.reset();
		g_game.setScreen(GAME_SCREEN_TITLE);
	}
}

void TpScreen::draw()
{
	g_course.draw();
	g_game.drawHUD();
	g_firework.draw();

	// Draw cake
	glBindTexture(
		GL_TEXTURE_2D,
		g_textures[TEXTURE_CAKE].m_texture);
	Rect(vec2(32, 32), vec2(16 * 7, 16 * 11)).draw();
	fontBegin();
	{
		for (int i = 0; i < MESSAGE_MAX; i++) {
			if (m_count < messages[i].startCount)
				break;
			fontPosition(messages[i].position.x, messages[i].position.y);
			fontDraw(messages[i].str);
		}
	}
	fontEnd();

	g_player.draw();
}