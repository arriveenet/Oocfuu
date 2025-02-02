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

	// BGMを一時停止
	static void pause();

	// BGMが再生中か判定
	static bool isPlaying();

	// BGMを設定
	static void setBgm(BgmType _bgm);

private:
	static BgmType m_bgmType;		//!< 再生するBGMの種別
	static SOUND_TYPE m_soundType;	//!< サウンドの種別
	static bool m_isPlaying;		//!< 再生中フラグ
};