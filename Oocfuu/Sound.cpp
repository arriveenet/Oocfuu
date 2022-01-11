#include "Sound.h"
#include "audio.h"
#include <stdio.h>

Sound g_sound[SOUND_MAX] = {
	{"sound\\yo.wav"},
	{"sound\\hoi.wav"},
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
	audioLoadWaveFile(fileName, &m_bid);

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
	for (int i = 0; i < SOUND_MAX; i++) {
		g_sound[i].init();
	}
	return 0;
}