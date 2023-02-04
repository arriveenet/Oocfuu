#pragma once

#include "../score/score.h"
#include "Channel.h"

#define WHOLE_NOTE			4
#define HALF_NOTE			2
#define DOTTED_QUARTER_NOTE	1.5f
#define QUARTER_NOTE		1
#define EIGHTH_NOTE			.5f
#define SIXEENTH_NOTE		.25f

enum {
	MUSIC_CANON,
	MUSIC_ALWAYS_WITH_ME,
	MUSIC_EIGHT_MELODIES,
	MUSIC_TYPHOON_PARADE,
	MUSIC_HAPPY_BIRTHDAY_DAY,
	MUSIC_EXIT,
	MUSIC_MAX
};

struct Music {
	char m_title[32];
	bool m_play;
	int m_playCount;
	bool m_end;
	Channel m_channels[AUDIO_CHANNEL_MAX];

	Music();
	int init();
	void reset();
	void resetScore();
	void setScore(int _channel, SCORE *_score, int _count);
	void setTitle(const char* _title);
	void setGain(float _gain);
	void update();
	void draw();
	void play();
	void stop();
};

extern Music g_music;