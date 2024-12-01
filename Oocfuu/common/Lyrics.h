#pragma once
#include <string>
#include <vector>
#include "Actor.h"

struct Lyrics {
	std::string text;		// �̎��̃e�L�X�g
	unsigned int time;		// �\�����鎞��
};

class LyricsController : public Actor {
public:
	enum class State {
		Inital,
		Playing,
		Ended
	};

	LyricsController();
	~LyricsController();

	bool bindLyrics(const std::vector<Lyrics>& lyrics);

	void start();

	void reset()
	{
		m_state = State::Inital;
		m_currentIndex = 0;
		m_count = 0;
	}

	void update() override;

	void draw() override;

protected:
	State m_state;
	unsigned int m_count;
	size_t m_currentIndex;
	std::string m_currentText;
	std::vector<Lyrics> m_lyrics;
};