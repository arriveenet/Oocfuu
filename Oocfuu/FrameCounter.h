#pragma once
#ifndef _FRAMECOUNTER_H
#define _FRAMECOUNTER_H

#include <time.h>

class FrameCounter
{
	clock_t m_startTime;
	unsigned int m_counter;
	unsigned int m_frameCount;

public:
	FrameCounter();

	unsigned int getFrameCount();
	void update();
};

extern FrameCounter g_frameCounter;

#endif // !_FRAMECOUNTER_H