#include "Header.h"
#include <time.h>
#include <string.h>

ivec2 windowSize;

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

	g_course.draw();

	fontBegin();
	{
		fontPosition(24, 16);
		fontDraw("OOCFUU");
		fontPosition(24, 24);
		fontDraw("%06d", 0);
		fontPosition(96, 24);
		fontDraw("x%02d", 0);
		fontPosition(144, 16);
		fontDraw("WORDL");
		fontPosition(152, 24);
		fontDraw("1-1");
		fontPosition(200, 16);
		fontDraw("TIME");
		fontPosition(40, 64);
		fontDraw("HAPPY BIRTHDAY OOCFUU!\n\n");
		fontDraw("HOPE YOU HAVE AN AMAZING\n\n");
		fontDraw("YEAR AHEAD!\n\n");
		fontDraw("FROM OSHU-FUJIWARA-SHI");
	}
	fontEnd();

	glutSwapBuffers();
}

void update()
{

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
		windowSize.y = SCREEN_HEIGHT * 3;
		windowSize.x = SCREEN_WIDTH * 3;
		glutInitWindowPosition(640, 200);
		glutInitWindowSize(windowSize.x, windowSize.y);
	}
	glutCreateWindow("title");

	fontInit(SCREEN_WIDTH, SCREEN_HEIGHT);
	Keyboard::init();

	g_sprite.loadBMPFile("resource\\CHR001.bmp", 0, 64, 128);
	g_parts->initAll();
	g_course.load("resource\\course.txt");

	glutDisplayFunc(display);
	glutTimerFunc(0, timer, 0);
	glutReshapeFunc(reshape);
	glutIgnoreKeyRepeat(GL_TRUE); //int ignore
	glutMainLoop();
}