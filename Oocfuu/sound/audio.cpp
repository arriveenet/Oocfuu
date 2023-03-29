#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <alc.h>
#include <al.h>

#include "audio.h"

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

	//ALCdevice* device = alcOpenDevice(NULL);// const ALCchar *devicename
	//if (device == NULL)
	//	return 1;

	//ALCcontext* context = alcCreateContext(
	//	device,	//ALCdevice *device
	//	NULL);	//const ALCint* attrlist
	//if (context == NULL)
	//	return 1;

	//alcMakeContextCurrent(context);// ALCcontext *context

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
		unsigned char* noise = new unsigned char[len];
		if (!noise) {
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
		delete[] noise;
	}

	/* Short noise */
	{
		const int len = 93;
		unsigned char* noise = new unsigned char[len];
		if (!noise) {
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
		delete[] noise;
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

void audioRelease()
{
	alDeleteBuffers(AUDIO_WAVEFORM_MAX, buffers);
	for (int i = 0; i < AUDIO_CHANNEL_MAX; i++) {
		alDeleteSources(1, &channels[i].sid);
	}
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
	const int divisorTable[] = { 4, 8, 16, 32, 64, 96, 128, 160,
		202, 254, 380, 508, 762, 1016, 2034, 4068 };
	return 1789772.5f / divisorTable[_index];
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