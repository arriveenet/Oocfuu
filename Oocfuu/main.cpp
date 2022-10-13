#include "Header.h"
#include "App.h"

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
	fontRelease();
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
		Keyboard::init();
		printf("GL_VERSION:%s\n", glGetString(GL_VERSION));   // GLenum name
		// MainLoop
		glutMainLoop();
	}
	release();

	return result;
}