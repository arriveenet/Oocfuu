#include "Header.h"
#include <time.h>
#include <string.h>

ivec2 windowSize;

void init()
{
	fontInit(SCREEN_WIDTH, SCREEN_HEIGHT);
	Keyboard::init();
	g_game.init();
	g_sprite.loadBMPFile("resource\\textures\\sprite\\CHR000.bmp", 0, 64, 128);
	g_parts->initAll();
	g_course.load("resource\\course\\course.txt");
	g_textures->initAll();
	g_player.init();
	g_firework.init();
	g_animations->initAll();
	g_sound->initAll();
	g_music.init();

	errno_t err;
	time_t t = time(NULL);
	err = localtime_s(&currentTime, &t);

	srand((unsigned int)time(NULL));
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(
		0, SCREEN_WIDTH,
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

	glutSwapBuffers();
}

void update()
{
	time_t t = time(NULL);
	localtime_s(&currentTime, &t);
	g_music.update();
	g_game.m_pCurrentScreen->update();

}

void timer(int value)
{
	audioUpdate();
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
	if (audioInit() != 0)
		return 1;

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
	glutMainLoop();
}