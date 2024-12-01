#include "Lyrics.h"
#include "Font.h"

LyricsController::LyricsController()
	: m_state(State::Inital)
	, m_count(0)
	, m_currentIndex(0)
{
}

LyricsController::~LyricsController()
{
}

bool LyricsController::bindLyrics(const std::vector<Lyrics>& lyrics)
{
	m_lyrics = lyrics;
	return true;
}

void LyricsController::start()
{
	m_state = State::Playing;
	m_currentIndex = 0;
	m_currentText = "";
}

void LyricsController::update()
{
	if (m_state != State::Playing)
		return;

	Lyrics& lyrics = m_lyrics[m_currentIndex];
	if (m_currentIndex >= m_lyrics.size() - 1 && m_count >= lyrics.time) {
		m_currentText = "";
		m_state = State::Ended;
		return;
	}

	if (m_count >= lyrics.time) {
		m_currentIndex++;
		m_count = 0;
		m_currentText = m_lyrics[m_currentIndex].text;
	}

	m_count++;
}

void LyricsController::draw()
{
	if ((m_state != State::Playing) || m_currentText.empty())
		return;

	fontBegin();
	fontPosition(0, 2);
	fontBitmapFontDraw(m_currentText.c_str());
	fontEnd();
}
