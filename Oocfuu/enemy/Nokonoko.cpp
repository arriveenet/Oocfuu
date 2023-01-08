#include "Nokonoko.h"

#include "TextureManager.h"

using namespace glm;
using std::vector;

const vec2 NOKONOKO_SIZE = vec2(16, 24);

Nokonoko::Nokonoko()
	: Nokonoko(0.0f, 0.0f)
{
}

Nokonoko::Nokonoko(glm::vec2 _position)
	: Nokonoko(_position.x, _position.y)
{
}

Nokonoko::Nokonoko(float _x, float _y)
	: m_dead(false)
	, m_falling(false)
	, m_state(NOKONOKO_STATE_MOVE)
	, m_counter(0)
	, m_speed(0, 0)
	, m_rightPoint(0, 0)
	, m_leftPoint(0, 0)
	, m_topPoint(0, 0)
	,Sprite(vec2(NOKONOKO_SIZE), vec2(_x, _y))
{
}

void Nokonoko::update()
{
}

void Nokonoko::draw()
{
	g_textureManager.setTexture(TEXTURE_NOKONOKO_RUN_1);
	Rect::draw();
	g_textureManager.unbindTexture();
}
