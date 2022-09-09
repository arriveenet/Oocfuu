#include "Header.h"
#include <time.h>
#include <string.h>

ivec2 windowSize;
clock_t startTime;
int FPS;
int FPSCount;

void printInit(int result, const char* _str)
{
	if (result == 0) {
		printf("[  OK  ] %s\n", _str);
	} else {
		printf("[FAILED] %s\n", _str);
	}
}

void init()
{
	fontInit();
	printInit(audioInit(), "Audio init");
	printInit(Keyboard::init(), "Keyboard init");
	printInit(g_game.init(), "Game init");
	printInit(g_parts->initAll(), "Part initAll");
	printInit(g_textures->initAll(), "Texture initAll");
	printInit(g_player.init(), "Player init");
	printInit(g_firework.init(), "Firework init");
	printInit(g_animations->initAll(), "Animation init");
	printInit(g_sound->initAll(), "Sound init");
	printInit(g_music.init(), "Music init");

	g_sprite.loadBMPFile("resource\\textures\\sprite\\CHR000.bmp", 0, 64, 128);
	g_course.load("resource\\course\\course1-1.txt");

	errno_t err;
	time_t t = time(NULL);
	err = localtime_s(&currentTime, &t);
	startTime = clock();

	srand((unsigned int)time(NULL));
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(
		g_course.m_scroll, g_course.m_scroll + SCREEN_WIDTH,
		SCREEN_HEIGHT, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);//GLenum cap
	glEnable(GL_BLEND);
	glBlendFunc(
		GL_SRC_ALPHA,			//GLenum sfactor
		GL_ONE_MINUS_SRC_ALPHA);//GLenum dfactor

	glColor3ub(0xff, 0xff, 0xff);

	g_game.m_pCurrentScreen->draw();
	/*
	fontBegin();
	{
		glColor3ub(0x00, 0xff, 0x00);
		fontPosition(0, 0);
		fontDraw("FPS:%d", FPS);
	}
	fontEnd();
	*/
	glutSwapBuffers();
}

void update()
{
	time_t t = time(NULL);
	localtime_s(&currentTime, &t);
	g_music.update();
	g_game.update();
	g_game.m_pCurrentScreen->update();

}

void timer(int value)
{
	audioUpdate();
	if (clock() - startTime >= 1000) {
		startTime = clock();
		FPS = FPSCount;
		FPSCount = 0;
	}
	FPSCount++;
	Keyboard::begin();
	update();
	Keyboard::end();
	glutPostRedisplay();

	glutTimerFunc(
		1000 / 60,// unsigned int time
		timer,	//void (* callback)( int )
		0);		// int value
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
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	{
		int windowWidth = GetSystemMetrics(SM_CXSCREEN);
		int windowHeight = GetSystemMetrics(SM_CYSCREEN);
		windowSize.y = 720 - 32;
		windowSize.x = windowSize.y * 4 / 3;
		glutInitWindowPosition(
			(windowWidth / 2) - (windowSize.x / 2),		// int x
			(windowHeight / 2) - (windowSize.y / 2));	// int y
		glutInitWindowSize(windowSize.x, windowSize.y);
	}
	glutCreateWindow("Happy Birthday to oocfuu!");

	// Initialize
	init();

	glutDisplayFunc(display);
	glutTimerFunc(0, timer, 0);
	glutReshapeFunc(reshape);
	glutIgnoreKeyRepeat(GL_TRUE); //int ignore
	printf("GL_VERSION:%s\n", glGetString(GL_VERSION));   // GLenum name
	// MainLoop
	glutMainLoop();
}