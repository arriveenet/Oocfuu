#include "Sound.h"
#include "Wave.h"

#include <alc.h>
#include <stdio.h>

#pragma comment(lib,"OpenAL32.lib")

static Wave* g_pWaveLoader = NULL;
Sound* g_pSound = NULL;

static const char* fileNames[SOUND_MAX] = {
	"yo.wav", //SOUND_SE_YO
	"hoi.wav", //SOUND_SE_HOI
	"firework.wav", //SOUND_FIREWORK
};

Sound::Sound()
	: m_bids(new ALuint[SOUND_MAX])
	, m_sids(new ALuint[SOUND_MAX])
{
}

Sound::~Sound()
{
	release();
}

Sound* Sound::getInstance()
{
	static Sound sound;
	return &sound;
}

bool Sound::init()
{
	ALCcontext* pContext = NULL;
	ALCdevice* pDevice = NULL;
	bool result = false;

	pDevice = alcOpenDevice(NULL);// const ALCchar *devicename
	if (pDevice) {
		pContext = alcCreateContext(
			pDevice,	// ALCdevice * device
			NULL);		// const ALCint * attrlist

		if (pContext) {
			printf("Opened %s Device\n", alcGetString(pDevice, ALC_DEVICE_SPECIFIER));
			alcMakeContextCurrent(pContext);// ALCcontext *context
			g_pWaveLoader = new Wave();
			result = true;
		} else {
			alcCloseDevice(pDevice);
		}
	}

	alGenBuffers(SOUND_MAX, m_bids);
	alGenSources(SOUND_MAX, m_sids);

	for (int i = 0; i < SOUND_MAX; i++) {

		if (loadWaveToBuffer(fileNames[i], m_bids[i])) {

			alSourcef(
				m_sids[i],		// ALuint source
				AL_GAIN,	// ALenum param
				0.2f);		// ALfloat value

			alSourcei(
				m_sids[i],			// ALuint sid
				AL_BUFFER,		// ALenum param
				m_bids[i]);			// ALint value

			result = true;
		}
	}

	return result;
}

void Sound::release()
{
	alDeleteBuffers(SOUND_MAX, m_bids);
	alDeleteSources(SOUND_MAX, m_sids);

	ALCcontext* pContext;
	ALCdevice* pDevice;

	pContext = alcGetCurrentContext();
	pDevice = alcGetContextsDevice(pContext);

	alcMakeContextCurrent(NULL);
	alcDestroyContext(pContext);
	alcCloseDevice(pDevice);

	if (g_pWaveLoader)
		delete g_pWaveLoader;

	if (m_bids)
		delete[] m_bids;

	if (m_sids)
		delete[] m_sids;
}

void Sound::play(int _sid)
{
	if(m_sids)
		alSourcePlay(m_sids[_sid]);
}

void Sound::stop(int _sid)
{
	if (m_sids)
		alSourceStop(m_sids[_sid]);
}

bool Sound::loadWaveToBuffer(const char* _fileName, ALuint _bid)
{
	unsigned long size;
	short* pData;
	unsigned int freq, format;
	bool result = false;
	if (g_pWaveLoader) {
		if (g_pWaveLoader->loadWaveFile(_fileName) == 0) {
			if ((g_pWaveLoader->getWaveSize(&size))
				&& (g_pWaveLoader->getWaveData((void**)&pData))
				&& (g_pWaveLoader->getWaveFrequency(&freq))
				&& (g_pWaveLoader->getWaveFormat(&format))
				)
			{
				/*
				printf("size=%d\n", size);
				printf("pData=%p\n", pData);
				printf("freq=%d\n", freq);
				printf("format=%#d\n", format);
				*/
				alBufferData(
					_bid,	// ALuint bid
					format,	// ALenum format
					pData,	// const ALvoid* data
					size,	// ALsizei size
					freq);	// ALsizei freq

				if (alGetError() == AL_NO_ERROR)
					result = true;

				g_pWaveLoader->deleteWaveFile();
			}
		}
	}

	return result;
}
