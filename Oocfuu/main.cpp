#include "Header.h"
#include "HappyBirthdayToYou.h"
#include "TyphoonParade.h"
#include <time.h>
#include <string.h>

enum {
	PHASE_HBD,
	PHASE_TP,
	PHASE_MAX
};

ivec2 windowSize;
int phase = PHASE_HBD;

typedef struct {
	char str[32];
	vec2 position;
	unsigned int startCount;
}MESSAGE;

float getCenter(int _len)
{
	return (float)(SCREEN_WIDTH / 2) - ((_len * 8) / 2);
}

static MESSAGE messages[] = {
	{"HAPPY BIRTHDAY OOCFUU!",vec2(getCenter(22), 64)},
	{"FEBRUARY,4,2022",vec2(120, 88)},
	{"THANK YOU FOR A FUN TIME.",vec2(getCenter(25),104)},
	{"HOPE YOU HAVE AN AMAZING",vec2(getCenter(24), 120)},
	{"YEAR AHEAD!",vec2(getCenter(24), 136)},
	{"FROM OSHU-FUJIWARA",vec2(96, 160)}
};

#define MESSAGE_MAX (sizeof(messages) / sizeof(MESSAGE))

void init()
{
	fontInit(SCREEN_WIDTH, SCREEN_HEIGHT);
	Keyboard::init();
	g_sprite.loadBMPFile("resource\\CHR000.bmp", 0, 64, 128);
	g_parts->initAll();
	g_course.load("resource\\course.txt");
	g_textures->initAll();
	g_player.init();
	g_animations->initAll();
	g_sound->initAll();
	g_music.init();
	g_music.setScore(AUDIO_CHANNEL_PULSE0, HBTY_Pulse0, 33);
	int total = 96;
	for (int i = 0; i < MESSAGE_MAX; i++) {
		messages[i].startCount = total;
		if (i == 3)
			continue;
		total += 96;
	}

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

	// Draw cake
	glBindTexture(
		GL_TEXTURE_2D,
		g_textures[TEXTURE_CAKE].m_texture);
	Rect(vec2(32,32), vec2(16*7, 16*11)).draw();

	switch (phase) {
	case PHASE_HBD:
		break;
	case PHASE_TP:
		fontBegin();
		{
			for (int i = 0; i < MESSAGE_MAX; i++) {
				if (g_game.m_count < messages[i].startCount)
					break;
				fontPosition(messages[i].position.x, messages[i].position.y);
				fontDraw(messages[i].str);
			}
		}
		fontEnd();
		break;
	}

	g_course.draw();
	g_player.draw();
	g_game.drawHUD();

	glutSwapBuffers();
}

void update()
{
	time_t t = time(NULL);
	localtime_s(&currentTime, &t);

	switch (phase) {
	case PHASE_HBD:
		if (g_music.m_end) {
			g_music.reset();
			g_music.setScore(AUDIO_CHANNEL_PULSE0, tpP0, 331);
			g_music.setScore(AUDIO_CHANNEL_PULSE1, tpP1, 430);
			g_music.setScore(AUDIO_CHANNEL_TRIANGLE, tpTri, 37);
			g_music.setScore(AUDIO_CHANNEL_NOISE, tpNoise, 4);
			g_music.play();
			phase = PHASE_TP;
		}
		break;
	case PHASE_TP:
		g_game.update();
		if (Keyboard::m_nowPressed['r']) {
			g_music.reset();
			g_music.play();
			g_game.m_count = 0;
		}
		if (Keyboard::m_nowPressed['k']) {
			g_music.m_play ? g_music.stop() : g_music.play();
		}
		break;
	}

	g_music.update();
	g_player.update();
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

	// Initialize
	init();

	g_music.play();

	glutDisplayFunc(display);
	glutTimerFunc(0, timer, 0);
	glutReshapeFunc(reshape);
	glutIgnoreKeyRepeat(GL_TRUE); //int ignore
	glutMainLoop();
}