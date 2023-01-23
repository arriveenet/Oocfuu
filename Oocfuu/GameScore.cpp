#include "GameScore.h"
#include "TextureManager.h"

GameScore::GameScore()
	: Rect(16.0f, 8.0f)
{
}

GameScore::~GameScore()
{
}

void GameScore::start(float _x, float _y)
{
	m_position = { _x, _y };
}

void GameScore::update()
{
}

void GameScore::draw()
{
	g_textureManager.setTexture(TEXTURE_SCORE_100);
	Rect::draw();
	g_textureManager.unbindTexture();
}
