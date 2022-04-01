#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <alc.h>
#include <al.h>

#include "audio.h"

#pragma comment(lib,"OpenAL32.lib")

#define WAVE_FORMAT_PCM			0x0001
#define WAVE_FORMAT_IEEE_FLOAT	0x0003
#define WAVE_FORMAT_ALAW		0x0006
#define WAVE_FORMAT_MULAW		0x0007
#define WAVE_FORMAT_EXTENSIBLE	0xFFFE

typedef struct
{
	char			szRIFF[4];
	unsigned long	ulRIFFSize;
	char			szWAVE[4];
} WAVEFILEHEADER;

typedef struct
{
	char			szChunkName[4];
	unsigned long	ulChunkSize;
} RIFFCHUNK;

typedef struct
{
	unsigned short	usFormatTag;
	unsigned short	usChannels;
	unsigned long	ulSamplesPerSec;
	unsigned long	ulAvgBytesPerSec;
	unsigned short	usBlockAlign;
	unsigned short	usBitsPerSample;
	unsigned short	usSize;
	unsigned short  usReserved;
	unsigned long	ulChannelMask;
} WAVEFMT;

static 	ALuint buffers[AUDIO_WAVEFORM_MAX];

//static float freqStart = DEFAULT_FREQ;

typedef struct {
	ALuint sid;
	int waveform;
	unsigned int length;
	unsigned int start;
	float decay;
	float startGain;
	float gain;
	float sweep;
	float freqStart;
	float freq;
	float freqEnd;
}CHANNEL;

CHANNEL channels[AUDIO_CHANNEL_MAX];

int audioInit() {

	ALCdevice* device = alcOpenDevice(NULL);// const ALCchar *devicename
	if (device == NULL)
		return 1;

	ALCcontext* context = alcCreateContext(
		device,	//ALCdevice *device
		NULL);	//const ALCint* attrlist
	if (context == NULL)
		return 1;

	alcMakeContextCurrent(context);// ALCcontext *context

	alGenBuffers(
		AUDIO_WAVEFORM_MAX,		// ALsizei n
		buffers);	// ALuint* buffers

	{
		unsigned char pulse[][8] = {
			{ 0xff, 0x00, 0x00 ,0x00 , 0x00 , 0x00 , 0x00 , 0x00},		// AUDIO_WAVEFORM_PULSE_12_5,
			{ 0xff, 0xff, 0x00 ,0x00 , 0x00 , 0x00 , 0x00 , 0x00},		// AUDIO_WAVEFORM_PULSE_25
			{ 0xff, 0xff, 0xff ,0xff , 0x00 , 0x00 , 0x00 , 0x00},		// AUDIO_WAVEFORM_PULSE_50
			{ 0xff, 0xff, 0xff ,0xff , 0xff , 0xff , 0x00 , 0x00}, };	// AUDIO_WAVEFORM_PULSE_75

		for (int i = AUDIO_WAVEFORM_PULSE_12_5; i <= AUDIO_WAVEFORM_PULSE_75; i++) {
			ALsizei size = sizeof pulse[0];
			alBufferData(
				buffers[i],				// ALuint bid
				AL_FORMAT_MONO8,		// ALenum format
				pulse[i],				// const ALvoid* data
				size,					// ALsizei size
				size);	// ALsizei freq
		}
	}

	{
		unsigned char triangle[] = {
			0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00,
			0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, };
		alBufferData(
			buffers[AUDIO_WAVEFORM_TRIANGLE],	// ALuint bid
			AL_FORMAT_MONO8,					// ALenum format
			triangle,							// const ALvoid* data
			sizeof triangle,					// ALsizei size
			sizeof triangle);				// ALsizei freq
	}

	/* Long noise */
	{
		const int len = 32767;
		unsigned char* noise;
		noise = (unsigned char*)calloc(len, sizeof(unsigned char));
		if (noise == NULL) {
			printf("audio: Failed to allocate memory.\n");
			return 1;
		}
		int shiftReg = 1 << 14;
		for (int i = 0; i < len; i++) {
			int result = (shiftReg ^ (shiftReg >> 1)) & 1;
			shiftReg >>= 1;
			shiftReg |= result << 14;
			noise[i] = 0xff * result;
		}
		alBufferData(
			buffers[AUDIO_WAVEFORM_NOISE_LONG],	// ALuint bid
			AL_FORMAT_MONO8,					// ALenum format
			noise,								// const ALvoid* data
			len,								// ALsizei size
			1);									// ALsizei freq
		free(noise);
	}

	/* Short noise */
	{
		const int len = 93;
		unsigned char* noise;
		noise = (unsigned char*)calloc(len, sizeof(unsigned char));
		if (noise == NULL) {
			printf("audio: Failed to allocate memory.\n");
			return 1;
		}
		int shiftReg = 1 << 14;
		for (int i = 0; i < len; i++) {
			int result = (shiftReg ^ (shiftReg >> 6)) & 1;
			shiftReg >>= 1;
			shiftReg |= result << 14;
			noise[i] = 0xff * result;
		}
		alBufferData(
			buffers[AUDIO_WAVEFORM_NOISE_SHORT],	// ALuint bid
			AL_FORMAT_MONO8,						// ALenum format
			noise,									// const ALvoid* data
			len,									// ALsizei size
			1);										// ALsizei freq
		free(noise);
	}

	for (int i = 0; i < AUDIO_CHANNEL_MAX; i++) {
		audioGain(i, AUDIO_DEFAULT_GAIN);

		alGenSources(
			1,					// ALsizei n
			&channels[i].sid);	// ALuint* sources

		alSourcei(
			channels[i].sid,	// ALuint sid
			AL_LOOPING,			// ALenum param
			AL_TRUE);			// ALint value
	}

	audioWaveform(AUDIO_CHANNEL_PULSE0, AUDIO_WAVEFORM_PULSE_12_5);
	audioWaveform(AUDIO_CHANNEL_PULSE1, AUDIO_WAVEFORM_PULSE_12_5);
	audioWaveform(AUDIO_CHANNEL_TRIANGLE, AUDIO_WAVEFORM_TRIANGLE);
	audioWaveform(AUDIO_CHANNEL_NOISE, AUDIO_WAVEFORM_NOISE_LONG);

	audioFreq(AUDIO_CHANNEL_PULSE0, AUDIO_DEFAULT_FREQ);
	audioFreq(AUDIO_CHANNEL_PULSE1, AUDIO_DEFAULT_FREQ);
	audioFreq(AUDIO_CHANNEL_TRIANGLE, AUDIO_DEFAULT_FREQ);
	audioFreq(AUDIO_CHANNEL_NOISE, audioIndexToFreq(8));

	return 0;
}

void audioWaveform(int _channel, int _waveform) {
	channels[_channel].waveform = _waveform;
	alSourcei(
		channels[_channel].sid,					// ALuint sid
		AL_BUFFER,								// ALenum param
		buffers[channels[_channel].waveform]);	// ALint value
}

void audioGain(int _channel, float _gain) {
	channels[_channel].gain = channels[_channel].startGain = _gain;
	alSourcef(
		channels[_channel].sid,		// ALuint sid
		AL_GAIN,					// ALenum param
		channels[_channel].gain);	// ALfloat value
}

void audioLength(int _channel, unsigned int _millis) {
	channels[_channel].length = _millis;
}

void audioDecay(int _channel, float _decay) {
	channels[_channel].decay = _decay;
}

void audioSweep(int _channel, float _sweep, float _freqEnd) {
	channels[_channel].sweep = _sweep;
	channels[_channel].freqEnd = _freqEnd;
}

void audioFreq(int _channel, float _freq) {
	channels[_channel].freq = channels[_channel].freqStart = _freq;
	alSourcef(
		channels[_channel].sid,		// ALuint sid
		AL_PITCH,					// ALenum param
		channels[_channel].freq);	// ALfloat value
}

float audioIndexToFreq(int _index) {
	int divisorTable[] = { 4, 8, 16, 32, 64, 96, 128, 160,
		202, 254, 380, 508, 762, 1016, 2034, 4068 };
	return 1789772.5f / divisorTable[_index];
}

static int parseFile(const char* _fileName, LPWAVEFILEINFO pWaveInfo)
{
	WAVEFILEHEADER	waveFileHeader;
	RIFFCHUNK		riffChunk;
	WAVEFMT			waveFmt;

	fopen_s(&pWaveInfo->pFile, _fileName, "rb");
	if (pWaveInfo->pFile) {
		//printf("The file %s was opened\n", _fileName);
		// Read Wave file header
		fread(&waveFileHeader, 1, sizeof(WAVEFILEHEADER), pWaveInfo->pFile);
		if (
			(!_strnicmp(waveFileHeader.szRIFF, "RIFF", 4))
			&& (!_strnicmp(waveFileHeader.szWAVE, "WAVE", 4))
			) {
			while (fread(&riffChunk, 1, sizeof(RIFFCHUNK), pWaveInfo->pFile) == sizeof(RIFFCHUNK)) {
				if (!_strnicmp(riffChunk.szChunkName, "fmt ", 4)) {
					if (riffChunk.ulChunkSize <= sizeof(WAVEFMT)) {
						fread(&waveFmt, 1, riffChunk.ulChunkSize, pWaveInfo->pFile);
						/*
						printf("fmtChunkSize:%d\n", riffChunk.ulChunkSize);
						printf("format:%d\n", waveFmt.usFormatTag);
						printf("channls:%d\n", waveFmt.usChannels);
						printf("samplesPerSec:%d\n", waveFmt.ulSamplesPerSec);
						printf("avgBytesPerSec:%d\n", waveFmt.ulAvgBytesPerSec);
						printf("blockAlign:%d\n", waveFmt.usBlockAlign);
						printf("bitsPerSample:%d\n", waveFmt.usBitsPerSample);
						printf("cbSize:%d\n", waveFmt.usSize);
						*/
						unsigned int format;
						if (audioGetFormat(waveFmt.usChannels, waveFmt.usBitsPerSample, &format)) {
							//printf("f=%#x\n", format);
							pWaveInfo->format = format;
						}
						pWaveInfo->freq = waveFmt.ulSamplesPerSec;
						// Determine if this is a WAVEFORMATEX or WAVEFORMATEXTENSIBLE wave file
						if (waveFmt.usFormatTag == WAVE_FORMAT_PCM) {
							//printf("format pcm\n");
							//pWaveInfo->wfType = WF_EX;
							//memcpy(&pWaveInfo->wfEXT.Format, &waveFmt, sizeof(PCMWAVEFORMAT));
						}
						else if (waveFmt.usFormatTag == WAVE_FORMAT_EXTENSIBLE) {
							//printf("format extensible\n");
							//pWaveInfo->wfType = WF_EXT;
							//memcpy(&pWaveInfo->wfEXT, &waveFmt, sizeof(WAVEFORMATEXTENSIBLE));
						}
					}
					else {
						fseek(pWaveInfo->pFile, riffChunk.ulChunkSize, SEEK_CUR);
					}
				}
				else if (!_strnicmp(riffChunk.szChunkName, "data", 4)) {
					//printf("dataSize=%s\n", riffChunk.szChunkName);
					//printf("dataSize=%d\n", riffChunk.ulChunkSize);
					pWaveInfo->dataSize = riffChunk.ulChunkSize;
					pWaveInfo->dataOffset = ftell(pWaveInfo->pFile);
					fseek(pWaveInfo->pFile, riffChunk.ulChunkSize, SEEK_CUR);
				}
				else {
					fseek(pWaveInfo->pFile, riffChunk.ulChunkSize, SEEK_CUR);
				}

				// Ensure that we are correctly aligned for next chunk
				if (riffChunk.ulChunkSize & 1)
					fseek(pWaveInfo->pFile, 1, SEEK_CUR);
			}

		}
		else {
			printf("Read faild wave file header.\n");
			return 1;
		}
	}
	else {
		printf("The file %s was not opened\n", _fileName);
		return 1;
	}

	return 0;
}

int audioLoadWaveFile(const char* _fileName, unsigned int* _bid)
{
	LPWAVEFILEINFO pWaveInfo;
	pWaveInfo = new WAVEFILEINFO;
	if (pWaveInfo) {
		if (parseFile(_fileName, pWaveInfo) == 0) {
			// Allocate memory for sample data
			pWaveInfo->pData = new short[pWaveInfo->dataSize / 2];
			if (pWaveInfo->pData) {
				// Seek to start of audio data
				fseek(pWaveInfo->pFile, pWaveInfo->dataOffset, SEEK_SET);
				if (fread(pWaveInfo->pData, sizeof(short), pWaveInfo->dataSize / 2, pWaveInfo->pFile) == pWaveInfo->dataSize / 2) {
					//printf("succsess\n");

					alBufferData(
						*_bid,// ALuint bid
						pWaveInfo->format,// ALenum format
						pWaveInfo->pData,// const ALvoid * data
						pWaveInfo->dataSize,// ALsizei size
						pWaveInfo->freq);// ALsizei freq
					ALenum error = alGetError();
					const ALchar* errorString = alGetString(error);
					//printf("%s\n", errorString);
					delete pWaveInfo->pData;
				}
			}
			fclose(pWaveInfo->pFile);
		} else {
			return 1;
		}
	}
	return 0;
}

bool audioGetFormat(int _channel, int _bitPerSample, unsigned int* _format)
{
	bool result = false;

	if (_format) {
		*_format = 0;

		if (_channel == 1) {
			switch (_bitPerSample) {
			case 8:
				*_format = AL_FORMAT_MONO8;
				break;
			case 16:
				*_format = AL_FORMAT_MONO16;
				break;
			}
		}
		else if (_channel == 2) {
			switch (_bitPerSample) {
			case 8:
				*_format = AL_FORMAT_STEREO8;
				break;
			case 16:
				*_format = AL_FORMAT_STEREO16;
				break;
			}
		}
		if (*_format == 0)
			result = false;
		else
			result = true;
	}
	else
		result = false;
	return result;
}

void audioPlay(int _channel) {
	channels[_channel].gain = channels[_channel].startGain;
	alSourcef(
		channels[_channel].sid,					// ALuint sid
		AL_GAIN,								// ALenum param
		channels[_channel].gain);// ALfloat value

	channels[_channel].freq = channels[_channel].freqStart;
	alSourcef(
		channels[_channel].sid,		// ALuint sid
		AL_PITCH,					// ALenum param
		channels[_channel].freq);	// ALfloat value

	alSourcePlay(channels[_channel].sid);	// ALuint sid
	channels[_channel].start = clock();

}

void audioStop(int _channel) {
	alSourceStop(channels[_channel].sid);	//  ALuint sid 
}

void audioUpdate() {
	for (int i = 0; i < AUDIO_CHANNEL_MAX; i++) {
		if ((channels[i].length > 0) && (clock() - channels[i].start >= channels[i].length))
			audioStop(i);

		if ((channels[i].decay != 0) && (channels[i].decay < 1)) {
			alSourcef(
				channels[i].sid,						// ALuint sid
				AL_GAIN,								// ALenum param
				channels[i].gain *= channels[i].decay);	// ALfloat value
		}

		if (channels[i].sweep != 0) {
			channels[i].freq *= channels[i].sweep;
			if (channels[i].freqEnd != 0) {
				if (
					((channels[i].sweep > 1) && (channels[i].freq >= channels[i].freqEnd))
					|| ((channels[i].sweep < 1) && (channels[i].freq <= channels[i].freqEnd))
					)
					audioStop(i);
			}
			alSourcef(
				channels[i].sid,	// ALuint sid
				AL_PITCH,			// ALenum param
				channels[i].freq);	// ALfloat value
		}
	}

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
		printf("%s\n",
			alGetString(error));// ALenum param
}