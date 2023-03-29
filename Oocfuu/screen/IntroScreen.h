#include "Screen.h"

enum INTRO {
	INTRO_LOAD,
	INTRO_GAMEOVER,
	INTRO_TIMEUP,
	INTRO_MAX,
};

class IntroScreen : public Screen {
	int m_counter = 0;
	INTRO m_intro = INTRO_LOAD;

public:
	void init() override;
	void reset() override;
	void update() override;
	void draw() override;
};