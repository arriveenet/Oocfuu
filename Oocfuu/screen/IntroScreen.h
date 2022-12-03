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
	virtual void init();
	virtual void reset();
	virtual void update();
	virtual void draw();
};