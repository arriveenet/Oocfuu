#include "Kinopio.h"
#include "TextureManager.h"
#include <glm/glm.hpp>


Kinopio::Kinopio()
	: m_enable(false)
	, Rect(glm::vec2(16.0f, 24.0f), glm::vec2(0.0f, 0.0f))
{
}

Kinopio::~Kinopio()
{
}

void Kinopio::draw()
{
	if (!m_enable)
		return;

	g_textureManager.setTexture(TEXTURE_KINOPIO);
	Rect::draw();
	g_textureManager.unbindTexture();
}
