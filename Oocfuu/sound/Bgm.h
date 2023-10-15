#pragma once
#include "sound/Sound.h"

/**
 * @brief BGMクラス
 * 
 */
class Bgm {
public:
	// BGMの種別
	enum BgmType {
		Unknown,
		Ground,
		Underground,
		Castle,
	};

	// コンストラクタ
	Bgm();
	// デストラクタ
	virtual ~Bgm();

	// BGMを再生
	static void play();

	// BGMを停止
	static void stop();

	// BGMを設定
	static void setBgm(BgmType _bgm);

private:
	static BgmType m_bgmType;		//!< 再生するBGMの種別
	static SOUNDTYPE m_soundType;	//!< サウンドの種別
};