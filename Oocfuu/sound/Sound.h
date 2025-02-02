#pragma once
#include <al.h>

typedef enum SOUND_TYPE {
	SOUND_TYPE_SE_YO,
	SOUND_TYPE_SE_HOI,
	SOUND_TYPE_SE_1UP,
	SOUND_TYPE_SE_COIN,
	SOUND_TYPE_SE_BREAK,
	SOUND_TYPE_SE_BUMP,
	SOUND_TYPE_SE_DIE,
	SOUND_TYPE_SE_KICK,
	SOUND_TYPE_SE_KOOPA_DIE,
	SOUND_TYPE_SE_ENEMY_FIRE,
	SOUND_TYPE_SE_PAUSE,
	SOUND_TYPE_SE_SQUISH,
	SOUND_TYPE_SE_FIREWORK,
	SOUND_TYPE_SE_FLAGPOLE,
	SOUND_TYPE_SE_FANFARE,
	SOUND_TYPE_SE_WORLED_CLEAR,
	SOUND_TYPE_BGM_GROUND,
	SOUND_TYPE_BGM_UNDERGROUND,
	SOUND_TYPE_BGM_CASTLE,
	SOUND_MAX
}SoundType;

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

	void play(SoundType _sid);
	void stop(SoundType _sid);
	void pause(SoundType _sid);
	void repeat(SoundType _sid, bool _enable = true);
	void setGain(SoundType _source, float _gain);
	ALint getState(SoundType _source);

private:
	bool loadWaveToBuffer(const char* _fileName, ALuint _bid);
};

extern Sound* g_pSound;