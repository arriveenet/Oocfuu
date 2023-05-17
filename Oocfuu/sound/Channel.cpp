#include "Channel.h"
#include "common/font.h"

#include <math.h>
#include <stdio.h>
#include "freeglut.h"

const char channelNames[][9] = {
	{"PULSE0"},
	{"PULSE1"},
	{"TRIANGLE"},
	{"NOISE"}
};

Channel::Channel()
	: m_score(NULL)
	, m_length(0)
	, m_phase(0)
	, m_count(0)
	, m_gain(AUDIO_DEFAULT_GAIN)
	, m_channel(0)
	, m_waveform(0)
	, m_end(false)
{
}

Channel::Channel(int _channel, int _waveform)
	: m_score(NULL)
	, m_length(0)
	, m_phase(0)
	, m_count(0)
	, m_gain(AUDIO_DEFAULT_GAIN)
	, m_channel(_channel)
	, m_waveform(_waveform)
	, m_end(false)
{
}

void Channel::setChannel(int _channel, int _waveform)
{
	m_channel = _channel;
	m_waveform = _waveform;
}

void Channel::setScore(SCORE _score[], int _count)
{
	m_count = --_count;
	m_score = _score;

	for (int i = 0; i <= m_count; i++) {
		if (m_score[i].decay == 0.0f)
			m_score[i].decay = .98f;
		int oct = m_score[i].key >= 3 && m_score[i].key < 12 ? m_score[i].oct - 1 : m_score[i].oct;
		if(m_channel != AUDIO_CHANNEL_NOISE)
			m_score[i].freq = score_octave[oct] * powf(2, m_score[i].key / 12.f);
		else
			m_score[i].freq = audioIndexToFreq(m_score[i].oct);
	}
}

void Channel::setGain(float _gain)
{
	if (_gain >= 0.0f && 1.0f >= _gain)
		m_gain = _gain;
}

void Channel::start()
{
	if (m_score == NULL) {
		//printf("%s channel is not set score!\n", channelNames[m_channel]);
		return;
	}
	audioStop(m_channel);
	audioWaveform(m_channel, m_waveform);
	audioFreq(m_channel, m_score[0].freq);
	audioDecay(m_channel, m_score[0].decay);
	audioGain(m_channel, m_gain);
	audioPlay(m_channel);
	m_end = false;
}

void Channel::reset()
{
	m_phase = 0;
	m_length = 0;
	m_end = false;
}

void Channel::resetScore()
{
	m_score = NULL;
}

void Channel::update()
{
	if ((m_phase >= m_count) || (m_score == NULL)) {
		m_end = true;
		return;
	}

	m_length++;

	if (m_length >= m_score[m_phase].length) {
		m_phase++;
		m_length = 0;
		if (m_score[m_phase].key == REST)
			audioStop(m_channel);
		else {
			audioStop(m_channel);
			audioWaveform(m_channel, m_waveform);
			audioFreq(m_channel, m_score[m_phase].freq);
			audioDecay(m_channel, m_score[m_phase].decay);
			audioGain(m_channel, m_gain);
			audioPlay(m_channel);
		}
	}
}

void Channel::draw(float _x, float _y)
{
	if (m_score == NULL)
		return;
	glColor3ub(0x00, 0xff, 0x00);
	fontBegin();
	fontPosition(_x + 8 * 1, _y);
	fontDraw("%s", channelNames[m_channel]);
	fontPosition(_x + 8 * 10, _y);
	fontDraw("%s", score_scale[m_score[m_phase].key]);
	fontPosition(_x + 8 * 14, _y);
	fontDraw("%d", m_score[m_phase].oct);
	fontPosition(_x + 8 * 18, _y);
	fontDraw("%.1f", m_score[m_phase].freq);
	fontPosition(_x + 8 * 27, _y);
	fontDraw("%d", m_score[m_phase].length);
	fontPosition(_x + 8 * 1, 100 +_y);
	fontEnd();
}

bool Channel::isEnd()
{
	return m_end;
}