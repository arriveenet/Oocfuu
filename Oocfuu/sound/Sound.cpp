#include "Sound.h"
#include "Wave.h"

#include <alc.h>
#include <stdio.h>

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#pragma comment(lib,"OpenAL32.lib")

static Wave* g_pWaveLoader = NULL;
Sound* g_pSound = NULL;

static const char* fileNames[SOUND_MAX] = {
	"resource\\sound\\Yo.wav",				// SOUND_TYPE_SE_YO
	"resource\\sound\\Hoi.wav",				// SOUND_TYPE_SE_HOI
	"resource\\sound\\1up.wav",				// SOUND_TYPE_SE_1UP
	"resource\\sound\\Coin.wav",			// SOUND_TYPE_SE_COIN
	"resource\\sound\\Break.wav",			// SOUND_TYPE_SE_BREAK
	"resource\\sound\\Bump.wav",			// SOUND_TYPE_SE_BUMP
	"resource\\sound\\Die.wav",				// SOUND_TYPE_SE_DIE
	"resource\\sound\\Kick.wav",			// SOUND_TYPE_SE_KICK
	"resource\\sound\\KoopaDie.wav",		// SOUND_TYPE_SE_KOOPA_DIE
	"resource\\sound\\EnemyFire.wav",		// SOUND_TYPE_SE_ENEMY_FIRE
	"resource\\sound\\Pause.wav",			// SOUND_TYPE_SE_PAUSE
	"resource\\sound\\Squish.wav",			// SOUND_TYPE_SE_SQUISH
	"resource\\sound\\Firework.wav",		// SOUND_TYPE_SE_FIREWORK
	"resource\\sound\\Flagpole.wav",		// SOUND_TYPE_SE_FLAGPOLE
	"resource\\sound\\Fanfare.wav",			// SOUND_TYPE_SE_FANFARE
	"resource\\sound\\WorldClear.wav",		// SOUND_TYPE_SE_WORLED_CLEAR
	"resource\\sound\\GroundBGM.wav",		// SOUND_TYPE_BGM_GROUND
	"resource\\sound\\UndergroundBGM.wav",	// SOUND_TYPE_BGM_UNDERGROUND
	"resource\\sound\\CastleBGM.wav",		// SOUND_TYPE_BGM_CASTLE,
};

Sound::Sound()
	: m_bids(NULL)
	, m_sids(NULL)
{
}

Sound::~Sound()
{
	if (g_pWaveLoader) {
		delete g_pWaveLoader;
		g_pWaveLoader = NULL;
	}
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
			//printf("Opened %s Device\n", alcGetString(pDevice, ALC_DEVICE_SPECIFIER));
			alcMakeContextCurrent(pContext);// ALCcontext *context
			g_pWaveLoader = new Wave();
			result = true;
		} else {
			alcCloseDevice(pDevice);
		}
	}

	m_bids = new ALuint[SOUND_MAX];
	m_sids = new ALuint[SOUND_MAX];

	alGenBuffers(SOUND_MAX, m_bids);
	alGenSources(SOUND_MAX, m_sids);

	for (int i = 0; i < SOUND_MAX; i++) {

		if (loadWaveToBuffer(fileNames[i], m_bids[i])) {

			alSourcef(
				m_sids[i],		// ALuint source
				AL_GAIN,	// ALenum param
				1.0f);		// ALfloat value

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

	if (g_pWaveLoader) {
		delete g_pWaveLoader;
		g_pWaveLoader = NULL;
	}

	if (m_bids) {
		delete[] m_bids;
		m_bids = NULL;
	}

	if (m_sids) {
		delete[] m_sids;
		m_sids = NULL;
	}
}

void Sound::play(SoundType _sid)
{
	if(m_sids)
		alSourcePlay(m_sids[_sid]);
}

void Sound::stop(SoundType _sid)
{
	if (m_sids)
		alSourceStop(m_sids[_sid]);
}

void Sound::repeat(SoundType _sid, bool _enable)
{
	alSourcei(
		m_sids[_sid],						// ALuint sid
		AL_LOOPING,							// ALenum param
		_enable ? AL_TRUE : AL_FALSE);		//  ALint value
}

void Sound::setGain(SoundType _source, float _gain)
{
	alSourcef(
		m_sids[_source],	// ALuint source
		AL_GAIN,			// ALenum param
		_gain);				// ALfloat value
}

ALint Sound::getState(SoundType _source)
{
	ALint value;
	alGetSourcei(
		m_sids[_source],	// ALuint sid
		AL_SOURCE_STATE,	// ALenum param
		&value);			// ALint * value
	return value;
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
				/*printf("size=%d\n", size);
				printf("pData=%p\n", pData);
				printf("freq=%d\n", freq);
				printf("format=%#d\n", format);*/
				
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
