#include "Sprite.h"

Sprite::Sprite()
	: m_animeCtr()
{
}

Sprite::Sprite(glm::vec2 _size, glm::vec2 _position)
	: m_animeCtr()
	, Rect(_size, _position)
{
}

Sprite::~Sprite()
{
}

void Sprite::update()
{
}

void Sprite::draw()
{
	Rect::draw();
}
