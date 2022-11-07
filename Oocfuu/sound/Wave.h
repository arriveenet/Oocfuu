#pragma once

#include <stdio.h>

typedef struct {
	FILE* pFile;
	signed short* pData;
	unsigned int format;
	unsigned long dataSize;
	unsigned long dataOffset;
	unsigned int freq;
}WAVEFILEINFO, * LPWAVEFILEINFO;

class Wave
{
public:
	Wave();
	~Wave();

	int loadWaveFile(const char* _fileName);
	bool getWaveData(void** _data);
	bool getWaveSize(unsigned long* _size);
	bool getWaveFrequency(unsigned int* _freq);
	bool getWaveFormat(unsigned int* _format);
	bool deleteWaveFile();

private:
	int parseFile(const char* _fileName, LPWAVEFILEINFO pWaveInfo);
	bool getWaveALBufferFormat(int _channel, int _bitPerSample, unsigned int* _format);
	LPWAVEFILEINFO m_wave;
	bool m_isWave;
};

