#include "Bgm.h"

Bgm::BgmType Bgm::m_bgmType;
SOUND_TYPE Bgm::m_soundType;

/**
 * @brief �R���X�g���N�^
 *
 * @return �Ȃ�
 * 
 */
Bgm::Bgm()
{
	m_bgmType = Unknown;
}

/**
 * @brief �f�X�g���N�^
 *
 * @return �Ȃ�
 * 
 */
Bgm::~Bgm()
{
}

/**
 * @brief BGM���Đ�����
 *
 * @return �Ȃ�
 *
 */
void Bgm::play()
{
	// BGM���s���̏ꍇ�A�Đ����Ȃ�
	if (m_bgmType == Unknown)
		return;

	g_pSound->repeat(m_soundType);
	g_pSound->play(m_soundType);
}

/**
 * @brief BGM���~����
 *
 * @return �Ȃ�
 *
 */
void Bgm::stop()
{
	g_pSound->stop(m_soundType);
}

/**
 * @brief BGM��ݒ肷��
 *
 * @param _bgm �ݒ肷��BGM���
 * 
 * @return �Ȃ�
 *
 */
void Bgm::setBgm(BgmType _bgm)
{
	m_bgmType = _bgm;

	switch (_bgm) {
	case Bgm::Ground:
		m_soundType = SOUND_TYPE_BGM_GROUND;
		break;
	case Bgm::Underground:
		m_soundType = SOUND_TYPE_BGM_UNDERGROUND;
		break;
	case Bgm::Castle:
		m_soundType = SOUND_TYPE_BGM_CASTLE;
		break;
	default:
		break;
	}
}
