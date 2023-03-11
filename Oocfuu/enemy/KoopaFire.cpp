#include "KoopaFire.h"

#include "Game.h"
#include "sound/Sound.h"
#include "world/Course.h"
#include "common/common.h"


KoopaFire::KoopaFire()
	: m_enable(false)
	, m_texture(TEXTURE_KOOPA_FIRE_1)
	, m_speed(0.0f, 0.0f)
	, m_height(0.0f)
	, Rect(24, 8)
{ }

KoopaFire::~KoopaFire()
{ }

void KoopaFire::start(glm::vec2 _position, float _height, int _direction)
{
	m_enable = true;
	m_position = _position;
	// ファイヤーの高さをセット
	m_height = _height;

	// スピードとテクスチャの向きをセット
	if (_direction == RECT_FLIP_NONE) {
		m_speed.x = -KOOPA_FIRE_SPEED;
		m_flip = RECT_FLIP_NONE;
	}
	else {
		m_speed.x = KOOPA_FIRE_SPEED;
		m_flip = RECT_FLIP_HORIZONTAL;
	}

	// ファイヤーの効果音を再生
	g_pSound->play(SOUND_SE_ENEMY_FIRE);
}

void KoopaFire::reset()
{
	m_enable = false;
}

void KoopaFire::update()
{
	if (!m_enable)
		return;

	if (m_position.y < m_height) {
		m_position.y += 1.0f;
	}

	m_position += m_speed;

	if (Game::m_count % 2 == 0)
		m_texture = (m_texture == TEXTURE_KOOPA_FIRE_1) ? TEXTURE_KOOPA_FIRE_2 : TEXTURE_KOOPA_FIRE_1;

	// 画面外に出たら更新を終了する
	if ((m_flip == RECT_FLIP_NONE)
		&& (m_position.x + m_size.x <= g_courseManager.getScroll())) {
 		m_enable = false;
	}
	if ((m_flip == RECT_FLIP_HORIZONTAL)
		&& (m_position.x >= g_courseManager.getScroll() + SCREEN_WIDTH)) {
		m_enable = false;
	}
}

void KoopaFire::draw()
{
	if (!m_enable)
		return;

	g_textureManager.setTexture(m_texture);
	Rect::draw();
	g_textureManager.unbindTexture();

	if (Game::m_debugInfo) {
		Rect::drawWire();
	}
}

bool KoopaFire::intersect(Rect const& _rect)
{
	// ファイヤーが有効であれば当たり判定を取らない
	if (!m_enable)
		return false;

	return Rect::intersect(_rect);
}
