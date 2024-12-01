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
 * @brief �~���[�W�b�N���
 */
enum class MusicType {
	Unknown,		//!< �s��
	HappyBirthday,	//!< �n�b�s�[�o�[�X�f�[
	TyphoonParade,	//!< �^�C�t�[���p���[�h
	Voyager,		//!< VOYAGER���t�̂Ȃ���W
	Canon,			//!< �J�m��
	OneLove,		//!< One Love
};

/**
 * @brief �~���[�W�b�N�̏��
 */
enum class MusicState {
	Initial,	//!< �Đ��O
	Playing,	//!< �Đ���
	Paused,		//!< �ꎞ��~��
	Stopped		//!< �Đ��I����
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
	MusicType m_type;		//!< �~���[�W�b�N�̎��
	MusicState m_state;		//!< �~���[�W�b�N�̏��
};

extern Music g_music;