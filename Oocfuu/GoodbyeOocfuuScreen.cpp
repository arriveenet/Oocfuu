#include "GoodbyeOocfuuScreen.h"
#include "Keyboard.h"
#include "Music.h"
#include "Game.h"
#include "font.h"
#include "Texture.h"
#include "Player.h"
#include "Course.h"

#include <freeglut.h>

void GoodbyeOocfuuScreen::init()
{

}

void GoodbyeOocfuuScreen::reset()
{

}

void GoodbyeOocfuuScreen::update()
{
	g_player.update();

	if (Keyboard::m_nowPressed['r']) {
		g_music.reset();
		g_music.play();
	}
	if (Keyboard::m_nowPressed['k']) {
		g_music.m_play ? g_music.stop() : g_music.play();
	}
}

void GoodbyeOocfuuScreen::draw()
{
	g_course.draw();
	g_game.drawHUD();

	fontBegin();
	fontPosition(68, 128);
	fontDraw("GOODBYE OOCFUU.");
	fontEnd();
/*
	{
		int oocfuuIndex = TEXTURE_HOI;
		int animationTable[] = { 0,1,2 };
		int animationTableLength = sizeof(animationTable) / sizeof(int);
		oocfuuIndex += animationTable[(g_game.m_count / 240) % animationTableLength];
		GLuint texture = g_textures[oocfuuIndex].m_texture;
		glBindTexture(
			GL_TEXTURE_2D,
			texture);
		Rect(vec2(28, 28), vec2(0, 180)).draw();
	}
	*/

	g_player.draw();
}