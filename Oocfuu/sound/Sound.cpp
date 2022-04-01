#include "Sound.h"
#include "audio.h"
#include <stdio.h>

Sound g_sound[SOUND_MAX] = {
	{"sound\\yo.wav"},
	{"sound\\hoi.wav"},
	{"sound\\firework.wav"}
};

Sound::~Sound()
{
}

int Sound::init()
{
	alGenBuffers(
		1,			// ALsizei n
		&m_bid);	// ALuint* buffers
	char fileName[256] = "resource\\";
	sprintf_s(fileName, "%s%s", fileName, m_fileName);
	//printf("%s\n", fileName);
	if (audioLoadWaveFile(fileName, &m_bid) != 0)
		return 1;

	alGenSources(
		1,			// ALsizei n
		&m_sid);	// ALuint* sources

	alSourcei(
		m_sid,			// ALuint sid
		AL_BUFFER,		// ALenum param
		m_bid);			// ALint value
	return 0;
}

int Sound::initAll()
{
	int failedCount = 0;
	for (int i = 0; i < SOUND_MAX; i++) {
		if (g_sound[i].init() != 0)
			failedCount++;
	}
	if (failedCount > 0)
		return 1;

	return 0;
}