#include "Bgm.h"

Bgm::BgmType Bgm::m_bgmType;
SOUND_TYPE Bgm::m_soundType;
bool Bgm::m_isPlaying;

/**
 * @brief �R���X�g���N�^
 *
 * @return �Ȃ�
 * 
 */
Bgm::Bgm()
{
	m_bgmType = Unknown;
	m_isPlaying = false;
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

	//m_isPlaying = true;
	//g_pSound->repeat(m_soundType);
	//g_pSound->play(m_soundType);
}

/**
 * @brief BGM���~����
 *
 * @return �Ȃ�
 *
 */
void Bgm::stop()
{
	m_isPlaying = false;
	g_pSound->stop(m_soundType);
}

/**
 * @brief BGM���Đ���������
 *
 * @return �Đ����Ftrue�A��~���Ffalse
 *
 */
bool Bgm::isPlaying()
{
	return m_isPlaying;
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
