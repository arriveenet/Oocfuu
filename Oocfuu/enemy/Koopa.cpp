#include "Koopa.h"
#include "Course.h"
#include "Part.h"
#include "player/Player.h"
#include "TextureManager.h"

using namespace glm;
using namespace std;

Koopa::Koopa()
	: Koopa(0.0f, 0.0f)
{
}

Koopa::Koopa(glm::vec2 _position)
	: Koopa(_position.x, _position.y)
{
}

Koopa::Koopa(float _x, float _y)
	: m_dead(false)
	, m_falling(false)
	, m_counter(0)
	, m_state(KOOPAP_STATE_IDLE)
	, m_speed(0, 0)
	, Sprite(vec2(KOOPA_WIDTH, KOOPA_HEIGHT), vec2(_x, _y))
{
}

Koopa::~Koopa()
{
}

void Koopa::update()
{
	m_counter++;
	m_flip = m_position.x < g_player.m_position.x ? RECT_FLIP_HORIZONTAL : RECT_FLIP_NONE;

	switch (m_state) {
	case KOOPAP_STATE_IDLE:
		break;
	case KOOPAP_STATE_FRONTJUMP:
		break;
	case KOOPAP_STATE_BACKJUMP:
		break;
	case KOOPAP_STATE_BREATH:
		break;
	case KOOPAP_STATE_MAX:
		break;
	default:
		break;
	}

	if (m_falling)
		m_speed.y += KOOPA_FALL_SPEED;

	m_position += m_speed;

	m_bottomPoints.clear();

	m_bottomPoints.push_back(m_position + vec2(1, m_size.y));
	m_bottomPoints.push_back(m_position + vec2(m_size.x - 1, m_size.y));

	m_falling = true;
	if (m_speed.y >= 0)
		for (vector<vec2>::iterator iter = m_bottomPoints.begin();
			iter != m_bottomPoints.end();
			iter++) {
		if (g_courseManager.intersect(*iter)) {
			vec2 bottom = ((ivec2)*iter / PART_SIZE) * PART_SIZE;
			m_position.y = bottom.y - KOOPA_HEIGHT;
			m_speed.y = 0;
			m_falling = false;
			break;
		}
	}
}

void Koopa::draw()
{
	g_textureManager.setTexture(TEXTURE_KOOPA_IDLE_1);
	Rect::draw();
	g_textureManager.unbindTexture();
}
