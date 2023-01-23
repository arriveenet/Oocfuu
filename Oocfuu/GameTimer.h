#pragma once
#define GAME_START_TIME				400
#define GAME_TIMER_RATE				24
#define GAME_TIMER_COUNTDOWN_RATE	1

// ゲームタイマークラス
class GameTimer {
private:
	short m_time;
	int m_rate;
	unsigned int m_count;
	bool m_isUpdate;
	bool m_visible;
	bool m_countDown;

public:
	GameTimer();
	~GameTimer();

	void update();
	void start(int _time = GAME_START_TIME);
	void stop();
	void countDown();
	short getTime() const;
	void setVisible(bool _visible);
	bool getVisible() const;
};

