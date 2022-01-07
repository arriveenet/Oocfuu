#include <time.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "music.h"
#include "audio.h"
#include "font.h"
#include "glut.h"

Music g_music;
int Music::init()
{
	strcpy_s(m_title, " ");
	m_play = false;
	m_playCount = 0;
	m_pulse0.setChannel(AUDIO_CHANNEL_PULSE0, AUDIO_WAVEFORM_PULSE_25);
	m_pulse1.setChannel(AUDIO_CHANNEL_PULSE1, AUDIO_WAVEFORM_PULSE_50);
	m_triangle.setChannel(AUDIO_CHANNEL_TRIANGLE, AUDIO_WAVEFORM_TRIANGLE);
	m_noise.setChannel(AUDIO_CHANNEL_NOISE, AUDIO_WAVEFORM_NOISE_LONG);

	return 0;
}

void Music::reset()
{
	stop();
	m_pulse0.reset();
	m_pulse1.reset();
	m_triangle.reset();
	m_noise.reset();
	m_playCount = 0;
}

void Music::resetScore()
{
	m_pulse0.resetScore();
	m_pulse1.resetScore();
	m_triangle.resetScore();
	m_noise.resetScore();
}

void Music::setScore(int _channel, SCORE* _score, int _count)
{
	switch (_channel){
	case AUDIO_CHANNEL_PULSE0:
		m_pulse0.setScore(_score, _count);
		break;
	case AUDIO_CHANNEL_PULSE1:
		m_pulse1.setScore(_score, _count);
		break;
	case AUDIO_CHANNEL_TRIANGLE:
		m_triangle.setScore(_score, _count);
		break;
	case AUDIO_CHANNEL_NOISE:
		m_noise.setScore(_score, _count);
		break;
	}
}

void Music::setTitle(const char* _title)
{
	strcpy_s(m_title, _title);
}

void Music::setGain(float _gain)
{
	m_pulse0.setGain(_gain);
	m_pulse1.setGain(_gain);
	m_triangle.setGain(_gain);
	m_noise.setGain(_gain);
}

void Music::update()
{
	if (!m_play)
		return;

	m_pulse0.update();
	m_pulse1.update();
	m_triangle.update();
	m_noise.update();
}


void Music::draw()
{
	m_pulse0.draw(0,8*2);
	m_pulse1.draw(0, 8 * 3);
	m_triangle.draw(0, 8 * 4);
	m_noise.draw(0, 8 * 5);

	fontBegin();
	glColor3ub(0xff, 0xff, 0xff);
	fontPosition(8 * 1, 5 * 1);
	fontDraw("CHANNEL");
	fontPosition(8 * 10, 5 * 1);
	fontDraw("KEY");
	fontPosition(8 * 14, 5 * 1);
	fontDraw("OCT");
	fontPosition(8 * 18, 5 * 1);
	fontDraw("FREQ");
	fontPosition(8 * 24, 5 * 1);
	fontDraw("LENGTH");
	fontPosition(8, 8 * 28);
	glColor3ub(0x00, 0xff, 0xff);
	fontDraw("%s", m_title);

	fontPosition(256-(5*8), 8 * 28);
	if (m_play)
		fontDraw("PLAY");
	else
		fontDraw("STOP");
	fontEnd();
}

void Music::play()
{
	if (m_playCount == 0) {
		m_pulse0.start();
		m_pulse1.start();
		m_triangle.start();
		m_noise.start();
	}

	m_playCount++;
	m_play = true;
}

void Music::stop()
{
	m_play = false;
	audioStop(AUDIO_CHANNEL_PULSE0);
	audioStop(AUDIO_CHANNEL_PULSE1);
	audioStop(AUDIO_CHANNEL_TRIANGLE);
	audioStop(AUDIO_CHANNEL_NOISE);
}