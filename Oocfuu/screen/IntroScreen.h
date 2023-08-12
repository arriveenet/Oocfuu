#include "Screen.h"

enum INTRO {
	INTRO_LOAD,
	INTRO_GAMEOVER,
	INTRO_TIMEUP,
	INTRO_MAX,
};

class IntroScreen : public Screen {
	int m_counter;
	INTRO m_intro;

public:
	IntroScreen();

	void init() override;
	void reset() override;
	void update() override;
	void draw() override;
};