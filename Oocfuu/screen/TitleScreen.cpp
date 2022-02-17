#include "TitleScreen.h"
#include "../Header.h"
#include <Windows.h>
#include <gl/glut.h>

#define TITLE_SCREEN_WIDTH	16
#define TITLE_SCREEN_HEIGHT	15

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
	: m_texture(0)
	, m_mode(MODE_1P)
{
	glGenTextures(
		1,				// GLsizei n
		&m_texture);	// GLuint *textures

	glBindTexture(
		GL_TEXTURE_2D,	// GLenum target
		m_texture);	// GLuint texture
	texFromBMP("resource\\textures\\item\\title_2.bmp", 0xff, 0x00, 0xff);

	glClearColor({ 80 / 255.f }, { 128 / 255.f }, { 255 / 255.f }, { 1 });
}

void TitleScreen::init()
{
}

void TitleScreen::reset()
{
	g_player.reset();
	g_player.m_left = 3;
	glClearColor({ 92 / 255.f }, { 148 / 255.f }, { 252 / 255.f }, { 1 });
	g_course.reload("resource\\course\\course1-1.txt", COURSE_WIDTH, COURSE_HEIGHT);
}
void TitleScreen::update()
{
	if (Keyboard::m_nowPressed['w'])
		m_mode--;
	if (Keyboard::m_nowPressed['s'])
		m_mode++;
	m_mode = (MODE_MAX + m_mode) % MODE_MAX;

	if (Keyboard::m_nowPressed['f'])
		switch (m_mode) {
		case MODE_1P:
			g_game.setScreen(GAME_SCREEN_INTRO);
			break;
		case MODE_HBD:
			g_game.setScreen(GAME_SCREEN_HBD);
			break;
		}

	if (Keyboard::m_nowPressed[0x1b]) {
		int onButton;
		onButton = MessageBox(
			NULL,
			TEXT("èIóπÇµÇƒÇ‡Ç¢Ç¢Ç≈Ç∑Ç©ÅH"),
			TEXT("Confirmation"),
			MB_YESNO | MB_ICONQUESTION);
		if (onButton == IDYES) {
			exit(0);
		}
	}

}

void TitleScreen::draw()
{
	g_course.draw();
	g_game.drawHUD();

	glBindTexture(
		GL_TEXTURE_2D,				// GLenum target
		m_texture);	// GLuint texture
	Rect rect(vec2(174, 88), vec2(40,32));
	rect.draw();

	g_player.draw(vec2(PLAYER_DEFAULT_X, PLAYER_DEFAULT_Y));

	fontBegin();
	fontPosition(120, 120);
	fontDraw("b2022 OOCFUU");
	fontPosition(72, 144);
	for (int i = 0; i < MODE_MAX; i++) {
		fontDraw("%s ", (i == m_mode) ? "a" : " ");
		fontDraw("%s\n", modeNames[i]);
		fontDraw("\n");
	}
	fontPosition(96, 184);
	fontDraw("TOP-%07d\n", 0);
	fontEnd();
}