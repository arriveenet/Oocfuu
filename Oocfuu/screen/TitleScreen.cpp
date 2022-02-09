#include "TitleScreen.h"
#include "../Header.h"
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
	: m_course(TITLE_SCREEN_WIDTH, TITLE_SCREEN_HEIGHT)
	, m_texture(0)
	, m_mode(MODE_1P)
{
	m_course.load("resource\\course\\title.txt");

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
	glClearColor({ 80 / 255.f }, { 128 / 255.f }, { 255 / 255.f }, { 1 });
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

}

void TitleScreen::draw()
{
	m_course.draw();
	g_game.drawHUD();

	glBindTexture(
		GL_TEXTURE_2D,				// GLenum target
		m_texture);	// GLuint texture
	Rect rect(vec2(174, 88), vec2(40,32));
	rect.draw();

	fontBegin();
	fontPosition(112, 120);
	fontDraw("b2022 ARRIVEE");
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