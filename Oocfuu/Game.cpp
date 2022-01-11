#include "Game.h"
#include "font.h"
#include "Rect.h"
#include "Texture.h"

#include <freeglut.h>

struct tm currentTime;
Game g_game;

void Game::update()
{
	m_count++;
}

void Game::drawHUD()
{
	GLuint texture = g_textures[TEXTURE_COIN_1].m_texture;
	glBindTexture(
		GL_TEXTURE_2D,	// GLenum target
		texture);		// GLuint texture
	Rect rect(vec2(8, 8), vec2(88, 24));
	rect.draw();

	fontBegin();
	{
		fontPosition(24, 16);
		fontDraw("OOCFUU");
		fontPosition(24, 24);
		fontDraw("%06d", currentTime.tm_year + 1900);
		fontPosition(96, 24);
		fontDraw("x%02d", currentTime.tm_sec);
		fontPosition(144, 16);
		fontDraw("WORLD");
		fontPosition(152, 24);
		fontDraw("%d-%d", currentTime.tm_mon + 1, currentTime.tm_mday);
		fontPosition(200, 16);
		fontDraw("TIME");
		fontPosition(200, 24);
		fontDraw("%02d:%02d", currentTime.tm_hour, currentTime.tm_min);
		fontPosition(40, 64);
	}
	fontEnd();
}