#include "TpScreen.h"
#include "Music.h"
#include "font.h"
#include "Keyboard.h"
#include "Game.h"
#include "Firework.h"
#include "Texture.h"
#include "Player.h"
#include "Course.h"

#include <freeglut.h>

#define SCREEN_WIDTH	256
#define SCREEN_HEIGHT	240

typedef struct {
	char str[32];
	vec2 position;
	unsigned int startCount;
}MESSAGE;

static float getCenter(int _len)
{
	return (float)(SCREEN_WIDTH / 2) - ((_len * 8) / 2);
}

static MESSAGE messages[] = {
	{"HAPPY BIRTHDAY OOCFUU!",vec2(getCenter(22), 64)},
	{"FEBRUARY,4,2022",vec2(120, 88)},
	{"THANK YOU FOR A FUN TIME.",vec2(getCenter(25),104)},
	{"HOPE YOU HAVE AN AMAZING",vec2(getCenter(24), 120)},
	{"YEAR AHEAD!",vec2(getCenter(24), 136)},
	{"FROM OSHU-FUJIWARA",vec2(96, 160)}
};

#define MESSAGE_MAX (sizeof(messages) / sizeof(MESSAGE))

void TpScreen::init()
{
	m_count = 0;
	int total = 96;
	for (int i = 0; i < MESSAGE_MAX; i++) {
		messages[i].startCount = total;
		if (i == 3)
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

	m_count++;
	if (g_music.m_end) {
		if (!g_firework.m_explosion) {
			int x = rand() % SCREEN_WIDTH;
			int y = rand() % SCREEN_HEIGHT;
			printf("x=%d, y=%d\n", x, y);
			g_firework.start(vec2(x, y));
		}
	}
	if (Keyboard::m_nowPressed['r']) {
		g_music.reset();
		g_music.play();
		m_count = 0;
	}
	if (Keyboard::m_nowPressed['k']) {
		g_music.m_play ? g_music.stop() : g_music.play();
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