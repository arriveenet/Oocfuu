/*
* Ç≥ÇÊÇ§Ç»ÇÁÅAÇ®Ç≠Ç”
*/

#include "GoodbyeOocfuuScreen.h"
#include "input/Keyboard.h"
#include "sound/Music.h"
#include "Game.h"
#include "common/font.h"
#include "Texture.h"
#include "player/Player.h"
#include "world/CourseManager.h"
#include "score/Voyager.h"

#include <freeglut.h>

void GoodbyeOocfuuScreen::init()
{

}

void GoodbyeOocfuuScreen::reset()
{
	g_music.reset();
	g_music.resetScore();

	g_music.play();
}

void GoodbyeOocfuuScreen::update()
{
	g_courseManager.update();

	if (Keyboard::m_nowPressed['r']) {
		g_music.reset();
		g_music.play();
	}
	if (Keyboard::m_nowPressed['k']) {
		g_music.m_play ? g_music.stop() : g_music.play();
	}

	if (Keyboard::m_nowPressed[0x1b])
		g_game.setScreen(GAME_SCREEN_TITLE);
}

void GoodbyeOocfuuScreen::draw()
{
	g_courseManager.draw();
	g_game.drawHUD();

	fontBegin();
	fontPosition(8*9, 8*12);
	glColor3ub(0xfe, 0xfe, 0xfe);
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

	//g_player.draw();
}