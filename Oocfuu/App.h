#pragma once
#include <time.h>

#define SCREEN_WIDTH	256
#define SCREEN_HEIGHT	240

class App {
public:
	tm m_currentTime;

	App();
	~App();

	bool init();
	void update();
	void draw();
};

extern App g_app;
