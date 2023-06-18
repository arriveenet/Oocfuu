#include <gl/freeglut.h>

#include <glm/glm.hpp>

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include "App.h"
#include "common/font.h"
#include "sound/audio.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"

using namespace glm;

ivec2 windowSize;

void display(void)
{
	// âΩÇ‡ÇµÇ»Ç¢ÅB

	//g_app.draw();

	//glutSwapBuffers();
}

void release(void)
{
	g_app.release();
}

void close(void)
{
	g_app.close();
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
	g_app.setWindow(glutCreateWindow("Happy Birthday to oocfuu!"));

	// Initialize
	if (!g_app.init())
		result = EXIT_FAILURE;
	else {
		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutCloseFunc(close);
		Keyboard::init();
		Mouse::init();
		printf("GL_VERSION:%s\n", glGetString(GL_VERSION));   // GLenum name

		// MainLoop
		g_app.run();
	}

	_CrtDumpMemoryLeaks();

	return result;
}