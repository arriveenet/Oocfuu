#pragma once

#include <al.h>
#include <alc.h>

enum {
	SOUND_SE_YO,
	SOUND_SE_HOI,
	SOUND_MAX
};

class Sound{
public:
	const char* m_fileName;
	unsigned int m_bid;
	unsigned int m_sid;

	~Sound();
	int init();
	static int initAll();
};

extern Sound g_sound[SOUND_MAX];