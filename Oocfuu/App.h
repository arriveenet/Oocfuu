#pragma once
#include <time.h>

#define SCREEN_WIDTH	256
#define SCREEN_HEIGHT	240

class App {
public:
	tm m_currentTime;

	App();
	virtual ~App();

	bool init();
	void release();
	void update();
	void draw();
};

extern App g_app;
