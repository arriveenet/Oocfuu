#pragma once
#include <time.h>
#include <Windows.h>

#define SCREEN_WIDTH	256
#define SCREEN_HEIGHT	240

class App {
private:
	HANDLE m_hConsoleOutput;
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
	
protected:
	bool printInit(int _result, const char* _str);
	bool printInit(bool _result, const char* _str);
};

extern App g_app;
