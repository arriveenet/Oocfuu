#pragma once
#include <al.h>

enum SOUNDTYPE {
	SOUND_SE_YO,
	SOUND_SE_HOI,
	SOUND_SE_DIE,
	SOUND_SE_SQUISH,
	SOUND_FIREWORK,
	SOUND_FLAGPOLE,
	SOUND_FANFARE,
	SOUND_MAX
};

class Sound {
	ALuint* m_bids;
	ALuint* m_sids;

	Sound();
	Sound(const Sound& _sound);
	~Sound();

public:
	static Sound* getInstance();

	bool init();
	void release();

	void play(SOUNDTYPE _sid);
	void stop(SOUNDTYPE _sid);
	void setGain(SOUNDTYPE _source, float _gain);

private:
	bool loadWaveToBuffer(const char* _fileName, ALuint _bid);
};

extern Sound* g_pSound;