#pragma once

class Mouse
{
private:
	Mouse();
	Mouse(const Mouse& _mouse);

	static Mouse* s_pMouse;

public:
	bool m_leftButton;
	bool m_rightButton;

	static void init();
	static Mouse* create();
	static void release();

};

extern Mouse* g_pMouse;
