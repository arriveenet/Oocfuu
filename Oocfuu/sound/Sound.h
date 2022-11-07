#pragma once
#include <al.h>

enum {
	SOUND_SE_YO,
	SOUND_SE_HOI,
	SOUND_FIREWORK,
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

	void play(int _sid);
	void stop(int _sid);

private:
	bool loadWaveToBuffer(const char* _fileName, ALuint _bid);
};

extern Sound* g_pSound;