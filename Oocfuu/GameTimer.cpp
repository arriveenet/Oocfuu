#include "GameTimer.h"

#include "Game.h"
#include "sound/Sound.h"

GameTimer::GameTimer()
	: m_time(0)
	, m_rate(GAME_TIMER_RATE)
	, m_count(0)
	, m_isUpdate(false)
	, m_visible(false)
	, m_countDown(false)
	, m_isEnded(false)
{
}

GameTimer::~GameTimer()
{
}

void GameTimer::update()
{
	m_count++;

	// タイマーをカウントダウンさせる
	if (m_isUpdate) {
		static int lastTime;
		int time = m_count / m_rate;
		if (time != lastTime) {
			m_time--;
			// ゴールのカウントダウン中であれば
			if (m_countDown) {
				g_pSound->play(SOUND_TYPE_SE_COIN);
				Game::getInstance()->addScore(50);
			}

			// カウントが終了した場合
			if (m_time < 0) {
				m_time = 0;
				m_isEnded = true;
				m_isUpdate = false;
			}
		}
		lastTime = time;
	}
}

void GameTimer::start(int _time)
{
	m_time = _time + 1;
	m_isUpdate = true;
	m_countDown = false;
	m_isEnded = false;
	m_rate = GAME_TIMER_RATE;
}

void GameTimer::stop()
{
	m_isUpdate = false;
}

void GameTimer::countDown()
{
	m_isUpdate = true;
	m_countDown = true;
	m_rate = GAME_TIMER_COUNTDOWN_RATE;
}

int GameTimer::getTime() const
{
	return m_time;
}

void GameTimer::setVisible(bool _visible)
{
	m_visible = _visible;
}

bool GameTimer::getVisible() const
{
	return m_visible;
}

bool GameTimer::isEnded() const
{
	return m_isEnded;
}
