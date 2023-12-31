#include "CourseObject.h"
#include "TextureManager.h"
#include <glm/glm.hpp>


Kinopio::Kinopio()
	: Sprite(glm::vec2(16.0f, 24.0f), glm::vec2(0.0f, 0.0f))
{
}

Kinopio::Kinopio(float x, float y)
	: Sprite(glm::vec2(16.0f, 24.0f), glm::vec2(x, y))
{
}

void Kinopio::draw()
{
	g_textureManager.setTexture(TEXTURE_KINOPIO);
	Rect::draw();
	g_textureManager.unbindTexture();
}

Cake::Cake()
	: Sprite(glm::vec2(26.0f, 26.0f), glm::vec2(0.0f, 0.0f))
{
}

Cake::Cake(float x, float y)
	: Sprite(glm::vec2(26.0f, 26.0f), glm::vec2(x, y))
{
}

void Cake::draw()
{
	g_textureManager.setTexture(TEXTURE_CAKE);
	Rect::draw();
	g_textureManager.unbindTexture();
}
