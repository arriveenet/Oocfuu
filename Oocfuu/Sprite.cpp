#include "Sprite.h"

Sprite::Sprite()
	: m_animationController()
{
}

Sprite::Sprite(const glm::vec2 _size, const glm::vec2 _position)
	: m_animationController()
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
