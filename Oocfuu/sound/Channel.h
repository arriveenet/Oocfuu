#pragma once
#include "score/score.h"

class Channel {
public:
	enum State {
		Inital,
		Playing,
		Paused,
		Stopped,
		Ended
	};

public:
	Channel();
	Channel(int _channel, int _waveform);
	void setChannel(int _channel, int _waveform);
	void setScore(SCORE _score[], int _count);
	void setGain(float _gain);
	void start();
	void reset();
	void resetScore();
	virtual void update();
	void draw(float _x, float _y);
	State getState() const;
	bool isEnd() const;

private:
	SCORE* m_score;
	unsigned int m_length = 0;
	int m_phase = 0;
	int m_count = 0;
	float m_gain;
	int m_channel;
	int m_waveform;
	State m_state;
};