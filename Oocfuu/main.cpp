#define FREEGLUT_STATIC
#include <gl/freeglut.h>

#include <glm/glm.hpp>

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

#include "App.h"
#include "sound/audio.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "font.h"

using namespace glm;

ivec2 windowSize;

void display(void)
{
	g_app.draw();

	glutSwapBuffers();
}

void timer(int value)
{
	audioUpdate();
	Keyboard::begin();

	g_app.update();

	Keyboard::end();
	glutPostRedisplay();

	glutTimerFunc(
		1000 / 60,// unsigned int time
		timer,	//void (* callback)( int )
		0);		// int value
}

void release()
{
	g_app.release();
}

void reshape(int width, int height)
{
	windowSize = ivec2(width, height);
	printf("reshape: width:%d height:%d\n", windowSize.x, windowSize.y);
	glViewport(
		0, 0,
		windowSize.x, windowSize.y
	);
}

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	int result = EXIT_SUCCESS;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	{
		int windowWidth = GetSystemMetrics(SM_CXSCREEN);
		int windowHeight = GetSystemMetrics(SM_CYSCREEN);
		windowSize.y = 720;
		windowSize.x = windowSize.y * 4 / 3;
		glutInitWindowPosition(
			(windowWidth / 2) - (windowSize.x / 2),		// int x
			(windowHeight / 2) - (windowSize.y / 2));	// int y
		glutInitWindowSize(windowSize.x, windowSize.y);
	}
	glutCreateWindow("Happy Birthday to oocfuu!");

	// Initialize
	if (!g_app.init())
		result = EXIT_FAILURE;
	else {
		glutDisplayFunc(display);
		glutTimerFunc(0, timer, 0);
		glutReshapeFunc(reshape);
		//glutCloseFunc(release);
		Keyboard::init();
		Mouse::init();
		printf("GL_VERSION:%s\n", glGetString(GL_VERSION));   // GLenum name
		// MainLoop
		glutMainLoop();
		//g_app.run();
	}
	release();

	_CrtDumpMemoryLeaks();

	return result;
}