#include "Header.h"
#include <time.h>

ivec2 windowSize;
Sprite sprite;

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

	{
		float f = (float)windowSize.x / SCREEN_WIDTH;
		glLineWidth(f);// GLfloat width
		glPointSize(f);// GLfloat size
	}

	glBindTexture(
		GL_TEXTURE_2D,
		sprite.m_textures['A']
	);
	Rect(8, 8).draw();

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
		windowSize.y = 720 - 32;
		windowSize.x = windowSize.y * 4 / 3;
		glutInitWindowPosition(640, 200);
		glutInitWindowSize(windowSize.x, windowSize.y);
	}
	glutCreateWindow("title");

	fontInit(SCREEN_WIDTH, SCREEN_HEIGHT);
	Keyboard::init();
	unsigned char alpha[3];
	alpha[0] = 0;
	alpha[1] = 57;
	alpha[2] = 115;
	sprite.loadBMPFile("CHR000.bmp", alpha);

	glutDisplayFunc(display);
	glutTimerFunc(0, timer, 0);
	glutReshapeFunc(reshape);
	glutIgnoreKeyRepeat(GL_TRUE); //int ignore
	glutMainLoop();
}