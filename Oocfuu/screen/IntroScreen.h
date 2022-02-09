#include "Screen.h"

class IntroScreen : public Screen {
	int m_counter = 0;

public:
	virtual void init();
	virtual void reset();
	virtual void update();
	virtual void draw();
};