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
	// �t�@�C���[�̍������Z�b�g
	m_height = _height;

	// �X�s�[�h�ƃe�N�X�`���̌������Z�b�g
	if (_direction == RECT_FLIP_NONE) {
		m_speed.x = -KOOPA_FIRE_SPEED;
		m_flip = RECT_FLIP_NONE;
	}
	else {
		m_speed.x = KOOPA_FIRE_SPEED;
		m_flip = RECT_FLIP_HORIZONTAL;
	}

	// �t�@�C���[�̌��ʉ����Đ�
	g_pSound->play(SOUND_SE_ENEMY_FIRE);
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

	// ��ʊO�ɏo����X�V���I������
	if ((m_position.x + m_size.x <= g_courseManager.m_scroll)
		|| (m_position.x >= g_courseManager.m_scroll + SCREEN_WIDTH)
	){
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
}