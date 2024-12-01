#pragma once

#include "Channel.h"
#include "score/score.h"
#include <string>

#define WHOLE_NOTE			4
#define HALF_NOTE			2
#define DOTTED_QUARTER_NOTE	1.5f
#define QUARTER_NOTE		1
#define EIGHTH_NOTE			.5f
#define SIXEENTH_NOTE		.25f

/**
 * @brief ミュージック種別
 */
enum class MusicType {
	Unknown,		//!< 不明
	HappyBirthday,	//!< ハッピーバースデー
	TyphoonParade,	//!< タイフーンパレード
	Voyager,		//!< VOYAGER日付のない墓標
	Canon,			//!< カノン
	OneLove,		//!< One Love
};

/**
 * @brief ミュージックの状態
 */
enum class MusicState {
	Initial,	//!< 再生前
	Playing,	//!< 再生中
	Paused,		//!< 一時停止中
	Stopped		//!< 再生終了後
};

struct Music {
	std::string m_title;
	bool m_play;
	int m_playCount;
	bool m_end;
	Channel m_channels[AUDIO_CHANNEL_MAX];

	Music();
	int init();
	void reset();
	void resetScore();
	void setMusic(MusicType type);
	void setScore(int _channel, SCORE *_score, int _count);
	void setTitle(const char* _title);
	void setGain(float _gain);
	void update();
	void draw();
	void play();
	void stop();
	MusicState getState() const;

private:
	MusicType m_type;		//!< ミュージックの種類
	MusicState m_state;		//!< ミュージックの状態
};

extern Music g_music;