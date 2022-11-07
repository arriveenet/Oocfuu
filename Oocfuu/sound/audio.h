#pragma once
#include <stdio.h>

enum {
	AUDIO_WAVEFORM_PULSE_12_5,
	AUDIO_WAVEFORM_PULSE_25,
	AUDIO_WAVEFORM_PULSE_50,
	AUDIO_WAVEFORM_PULSE_75,
	AUDIO_WAVEFORM_TRIANGLE,
	AUDIO_WAVEFORM_NOISE_LONG,
	AUDIO_WAVEFORM_NOISE_SHORT,
	AUDIO_WAVEFORM_MAX
};

enum {
	AUDIO_CHANNEL_PULSE0,
	AUDIO_CHANNEL_PULSE1,
	AUDIO_CHANNEL_TRIANGLE,
	AUDIO_CHANNEL_NOISE,
	AUDIO_CHANNEL_MAX
};

#define AUDIO_DEFAULT_GAIN	.1f
#define AUDIO_DEFAULT_FREQ	440

typedef struct {
	FILE* pFile;
	signed short* pData;
	unsigned int format;
	unsigned long dataSize;
	unsigned long dataOffset;
	unsigned int freq;
}WAVEFILEINFO, * LPWAVEFILEINFO;

int audioInit();
void audioWaveform(int _channel, int _waveform);
void audioGain(int _channel, float _gain);
void audioLength(int _channel, unsigned int _millis);
void audioDecay(int _channel, float _decay);
void audioSweep(int _channel, float _sweep, float _freqEnd = 0);
void audioFreq(int _channel, float _freq);
float audioIndexToFreq(int _index);
bool audioGetFormat(int _channel, int _bitPerSample, unsigned int* _format);
void audioPlay(int _channel);
void audioStop(int _channel);
void audioUpdate();