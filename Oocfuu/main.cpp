#include "Header.h"
#include "HappyBirthdayToYou.h"
#include "TyphoonParade.h"
#include <time.h>
#include <string.h>

ivec2 windowSize;
struct tm currentTime;

/* I NEED YOU */

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

	glBindTexture(
		GL_TEXTURE_2D,
		g_textures[TEXTURE_CAKE].m_texture);
	Rect(vec2(32,32), vec2(16*7, 16*11)).draw();

	fontBegin();
	{
		fontPosition(24, 16);
		fontDraw("OOCFUU");
		fontPosition(24, 24);
		fontDraw("%06d", currentTime.tm_year+1900);
		fontPosition(96, 24);
		fontDraw("X%02d", currentTime.tm_sec);
		fontPosition(144, 16);
		fontDraw("WORDLD");
		fontPosition(152, 24);
		fontDraw("%d-%d", currentTime.tm_mon+1, currentTime.tm_mday);
		fontPosition(200, 16);
		fontDraw("TIME");
		fontPosition(200, 24);
		fontDraw("%02d:%02d", currentTime.tm_hour, currentTime.tm_min);
		fontPosition(40, 64);
		fontDraw("HAPPY BIRTHDAY OOCFUU!\n\n");
		fontDraw("HOPE YOU HAVE AN AMAZING\n\n");
		fontDraw("YEAR AHEAD!\n\n");
		fontDraw("2022/02/04\n\n");
		fontDraw("FROM OSHU_FUJIWARA-SHI");
	}
	fontEnd();

	glutSwapBuffers();
}

void update()
{
	time_t t = time(NULL);
	localtime_s(&currentTime, &t);
	if (Keyboard::m_nowPressed['r']) {
		g_music.reset();
	}
	if (Keyboard::m_nowPressed[' ']) {
		g_music.m_play ? g_music.stop() : g_music.play();
	}
	g_music.update();
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
		windowSize.x = windowSize.y * 16 / 15;
		glutInitWindowPosition(
			(windowWidth / 2) - (windowSize.x / 2),		// int x
			(windowHeight / 2) - (windowSize.x / 2));	// int y
		glutInitWindowSize(windowSize.x, windowSize.y);
	}
	glutCreateWindow("Happy Birthday to oocfuu!");

	fontInit(SCREEN_WIDTH, SCREEN_HEIGHT);
	Keyboard::init();

	g_music.init();
	int p0Len = sizeof(tpP0) / sizeof(tpP0[0]);
	int p1Len = sizeof(tpP1) / sizeof(tpP1[0]);
	int triLen = sizeof(tpTri) / sizeof(tpTri[0]);
	int noiseLen = sizeof(tpNoise) / sizeof(tpNoise[0]);
	g_music.setScore(AUDIO_CHANNEL_PULSE0, tpP0, p0Len);
	g_music.setScore(AUDIO_CHANNEL_PULSE1, tpP1, p1Len);
	g_music.setScore(AUDIO_CHANNEL_TRIANGLE, tpTri, triLen);
	//g_music.setScore(AUDIO_CHANNEL_NOISE, tpNoise, noiseLen);
	g_music.play();
	g_sprite.loadBMPFile("resource\\CHR001.bmp", 0, 64, 128);
	g_parts->initAll();
	g_course.load("resource\\course.txt");
	g_textures->initAll();

	errno_t err;
	time_t t = time(NULL);
	err = localtime_s(&currentTime, &t);

	glutDisplayFunc(display);
	glutTimerFunc(0, timer, 0);
	glutReshapeFunc(reshape);
	glutIgnoreKeyRepeat(GL_TRUE); //int ignore
	glutMainLoop();
}