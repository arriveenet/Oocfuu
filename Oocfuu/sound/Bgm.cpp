#include "Bgm.h"

Bgm::BgmType Bgm::m_bgmType;
SOUND_TYPE Bgm::m_soundType;

/**
 * @brief コンストラクタ
 *
 * @return なし
 * 
 */
Bgm::Bgm()
{
	m_bgmType = Unknown;
}

/**
 * @brief デストラクタ
 *
 * @return なし
 * 
 */
Bgm::~Bgm()
{
}

/**
 * @brief BGMを再生する
 *
 * @return なし
 *
 */
void Bgm::play()
{
	// BGMが不明の場合、再生しない
	if (m_bgmType == Unknown)
		return;

	g_pSound->repeat(m_soundType);
	g_pSound->play(m_soundType);
}

/**
 * @brief BGMを停止する
 *
 * @return なし
 *
 */
void Bgm::stop()
{
	g_pSound->stop(m_soundType);
}

/**
 * @brief BGMを設定する
 *
 * @param _bgm 設定するBGM種別
 * 
 * @return なし
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
