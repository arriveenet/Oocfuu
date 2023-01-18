#pragma once
#include <time.h>

#define SCREEN_WIDTH	256
#define SCREEN_HEIGHT	240

class App {
	bool m_running;

public:
	tm m_currentTime;

	App();
	virtual ~App();

	bool init();
	void release();
	void update();
	void draw();
	void run();
};

extern App g_app;
