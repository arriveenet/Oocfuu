#include "Sprite.h"

Sprite::Sprite()
	: m_animationController()
{
}

Sprite::Sprite(const glm::vec2 _size, const glm::vec2 _position)
	: m_animationController()
{
	m_position = _position;
	m_size = _size;
	m_rect.m_position = _position;
	m_rect.m_size = _size;
}

Sprite::~Sprite()
{
}

void Sprite::update()
{
}

void Sprite::draw()
{
	m_rect.m_position = m_position;
	m_rect.m_size = m_size;
	m_rect.draw();
}

void Sprite::drawWire()
{
	m_rect.drawWire();
}

bool Sprite::intersect(const glm::vec2& point)
{
	return m_rect.intersect(point);
}

bool Sprite::intersect(const Rect& rect)
{
	return m_rect.intersect(rect);
}

bool Sprite::intersect(const Sprite& sprite)
{
	return m_rect.intersect(sprite.getRect());
}

bool Sprite::compare(const Sprite& sprite)
{
	return m_rect == sprite.getRect();
}

void Sprite::setFlip(int flip)
{
	m_rect.m_flip = flip;
}

bool Sprite::isFlipX() const
{
	return m_rect.m_flip == RECT_FLIP_HORIZONTAL;
}

bool Sprite::isFlipY() const
{
	return m_rect.m_flip == RECT_FLIP_VERTICAL;
}
