#include "Mouse.h"

#include <stdio.h>
#include <gl/glut.h>

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

Mouse* Mouse::s_pMouse = NULL;
Mouse* g_pMouse = NULL;

void mouse(int button, int state, int x, int y)
{
//	printf("button:%d\n", button);
//	printf("state:%d\n", state);
//	printf("xy:%d,%d\n", x, y);

	switch (button) {
	case GLUT_LEFT_BUTTON :
		if (state == GLUT_DOWN)
			g_pMouse->m_leftButton = true;
		else if (state == GLUT_UP)
			g_pMouse->m_leftButton = false;
		break;

	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			g_pMouse->m_rightButton = true;
		else if (state == GLUT_UP)
			g_pMouse->m_rightButton = false;
		break;

	case GLUT_MIDDLE_BUTTON:
	default:
		break;
	}
}

Mouse::Mouse()
	: m_leftButton(false)
	, m_rightButton(false)
{
}

Mouse::Mouse(const Mouse& _mouse)
	: m_leftButton(false)
	, m_rightButton(false)
{
}

void Mouse::init()
{
	g_pMouse = Mouse::create();
	glutMouseFunc(mouse);
}

Mouse* Mouse::create()
{
	if (!s_pMouse)
		s_pMouse = new Mouse;

	return s_pMouse;
}

void Mouse::release()
{
	if (s_pMouse) {
		delete s_pMouse;
		s_pMouse = NULL;
	}
}


