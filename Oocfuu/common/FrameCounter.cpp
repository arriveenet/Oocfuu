#include "FrameCounter.h"

FrameCounter g_frameCounter;

FrameCounter::FrameCounter()
	: m_counter(0)
	, m_frameCount(0)
{
	m_startTime = clock();
}

unsigned int FrameCounter::getFrameCount()
{
	return m_frameCount;
}

void FrameCounter::update()
{
	if (clock() - m_startTime >= 1000) {
		m_startTime = clock();
		m_frameCount = m_counter;
		//printf("FPS:%d\n", FPS);
		m_counter = 0;
	}
	m_counter++;
}