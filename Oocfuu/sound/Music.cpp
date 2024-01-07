#include "music.h"
#include "audio.h"
#include "common/font.h"
#include "score/HappyBirthdayToYou.h"
#include "score/TyphoonParade.h"
#include "score/TyphoonParade.h"
#include "score/Voyager.h"
#include "score/Canon.h"

#include <glut.h>

Music g_music;

Music::Music()
	: m_type(MusicType::Unknown)
	, m_state(MusicState::Initial)
	, m_play(false)
	, m_playCount(0)
	, m_end(false)
	, m_title("NO TITLE")
{}

int Music::init()
{
	m_play = false;
	m_playCount = 0;
	m_end = false;

	m_channels[AUDIO_CHANNEL_PULSE0].setChannel(AUDIO_CHANNEL_PULSE0, AUDIO_WAVEFORM_PULSE_50);
	m_channels[AUDIO_CHANNEL_PULSE1].setChannel(AUDIO_CHANNEL_PULSE1, AUDIO_WAVEFORM_PULSE_75);
	m_channels[AUDIO_CHANNEL_TRIANGLE].setChannel(AUDIO_CHANNEL_TRIANGLE, AUDIO_WAVEFORM_TRIANGLE);
	m_channels[AUDIO_CHANNEL_NOISE].setChannel(AUDIO_CHANNEL_NOISE, AUDIO_WAVEFORM_NOISE_LONG);

	return 0;
}

void Music::reset()
{
	stop();

	for (int i = 0; i < AUDIO_CHANNEL_MAX; i++)
		m_channels[i].reset();

	m_playCount = 0;
	m_end = false;
}

void Music::resetScore()
{
	for (int i = 0; i < AUDIO_CHANNEL_MAX; i++)
		m_channels[i].resetScore();

}

void Music::setMusic(MusicType _type)
{
	if (m_type != MusicType::Unknown) {
		resetScore();
	}

	m_type = _type;

	switch (_type) {
	case MusicType::Unknown:
		break;
	case MusicType::HappyBirthday:
		setScore(AUDIO_CHANNEL_PULSE0, pulse0_score_happy_birthday, PULSE0_COUNT_HAPPY_BIRTHDAY);
		setScore(AUDIO_CHANNEL_PULSE1, pulse1_score_happy_birthday, PULSE1_COUNT_HAPPY_BIRTHDAY);
		setScore(AUDIO_CHANNEL_TRIANGLE, triangle_score_happy_birthday, TRIANGLE_COUNT_HAPPY_BIRTHDAY);
		break;
	case MusicType::TyphoonParade:
		setScore(AUDIO_CHANNEL_PULSE0, pulse0_score_tyhoon_parade, PULSE0_COUNT_TYHOON_PARADE);
		setScore(AUDIO_CHANNEL_PULSE1, pulse1_score_tyhoon_parade, PULSE1_COUNT_TYHOON_PARADE);
		setScore(AUDIO_CHANNEL_TRIANGLE, triangle_score_tyhoon_parade, TRIANGLE_COUNT_TYHOON_PARADE);
		setScore(AUDIO_CHANNEL_NOISE, noise_score_tyhoon_parade, NOISE_COUNT_TYHOON_PARADE);
		break;
	case MusicType::Voyager:
		setScore(AUDIO_CHANNEL_PULSE0, pulse0_score_voyager, PULSE0_COUNT_VOYAGER);
		setScore(AUDIO_CHANNEL_PULSE1, pulse1_score_voyager, PULSE1_COUNT_VOYAGER);
		setScore(AUDIO_CHANNEL_TRIANGLE, triangle_score_voyager, TRIANGLE_COUNT_CANON);
		break;
	case MusicType::Canon:
		setScore(AUDIO_CHANNEL_PULSE0, pulse0_score_canon, PULSE0_COUNT_CANON);
		setScore(AUDIO_CHANNEL_PULSE1, pulse1_score_canon, PULSE1_COUNT_CANON);
		setScore(AUDIO_CHANNEL_TRIANGLE, triangle_score_canon, TRIANGLE_COUNT_CANON);
		break;
	default:
		break;
	}
}

void Music::setScore(int _channel, SCORE* _score, int _count)
{
	switch (_channel){
	case AUDIO_CHANNEL_PULSE0:
		m_channels[AUDIO_CHANNEL_PULSE0].setScore(_score, _count);
		break;
	case AUDIO_CHANNEL_PULSE1:
		m_channels[AUDIO_CHANNEL_PULSE1].setScore(_score, _count);
		break;
	case AUDIO_CHANNEL_TRIANGLE:
		m_channels[AUDIO_CHANNEL_TRIANGLE].setScore(_score, _count);
		break;
	case AUDIO_CHANNEL_NOISE:
		m_channels[AUDIO_CHANNEL_NOISE].setScore(_score, _count);
		break;
	}
}

void Music::setTitle(const char* _title)
{
	m_title = _title;
}

void Music::setGain(float _gain)
{
	for (int i = 0; i < AUDIO_CHANNEL_MAX; i++)
		m_channels[i].setGain(_gain);
}

void Music::update()
{
	if (!m_play)
		return;

	for (int i = 0; i < AUDIO_CHANNEL_MAX; i++)
		m_channels[i].update();

	if ((m_channels[AUDIO_CHANNEL_PULSE0].getState() == Channel::Ended)
		&& (m_channels[AUDIO_CHANNEL_PULSE1].getState() == Channel::Ended)
		&& (m_channels[AUDIO_CHANNEL_TRIANGLE].getState() == Channel::Ended)
		&& (m_channels[AUDIO_CHANNEL_NOISE].getState() == Channel::Ended)
		) {
		m_end = true;
		m_state = MusicState::Stopped;
	}
}


void Music::draw()
{
	m_channels[AUDIO_CHANNEL_PULSE0].draw(0,8*2);
	m_channels[AUDIO_CHANNEL_PULSE1].draw(0, 8 * 3);
	m_channels[AUDIO_CHANNEL_TRIANGLE].draw(0, 8 * 4);
	m_channels[AUDIO_CHANNEL_NOISE].draw(0, 8 * 5);

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
		for (int i = 0; i < AUDIO_CHANNEL_MAX; i++)
			m_channels[i].start();
	}

	m_playCount++;
	m_play = true;
	m_end = false;
	m_state = MusicState::Playing;
}

void Music::stop()
{
	m_state = MusicState::Paused;
	m_play = false;
	audioStop(AUDIO_CHANNEL_PULSE0);
	audioStop(AUDIO_CHANNEL_PULSE1);
	audioStop(AUDIO_CHANNEL_TRIANGLE);
	audioStop(AUDIO_CHANNEL_NOISE);
}

MusicState Music::getState() const
{
	return m_state;
}
