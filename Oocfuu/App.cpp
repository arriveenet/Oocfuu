#include "App.h"
#include "Game.h"
#include "util/timer.h"
#include "common/font.h"
#include "common/FrameCounter.h"
#include "world/Part.h"
#include "world/CourseManager.h"
#include "TextureManager.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "sound\audio.h"
#include "sound\Sound.h"
#include "sound\Music.h"
#include "sound\Channel.h"
#include "animation\Animation.h"
#include "animation\AnimationController.h"
#include "Player/Player.h"
#include "util/ZipUtils.h"

#include <stdio.h>
#include <gl/freeglut.h>

static constexpr float MS_PER_UPDATE = 1.0f / 60.0f;

App g_app;
extern glm::ivec2 windowSize;

/**
 * @breif �R���X�g���N�^
 */
App::App()
	: m_hConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE))
	, m_running(false)
	, m_window(0)
	, m_pGame(Game::getInstance())
	, m_deltaTime(0.0f)
	, m_accumulator(0.0f)
	, m_frames(0)
{
	m_currentTime = std::chrono::steady_clock::now();
}

/**
 * @breif �f�X�g���N�^
 */
App::~App()
{
}

/**
 * @breif �A�v���P�[�V�����̏��������s��
 * 
 * @param �Ȃ�
 * 
 * @return ����: true ���s: false
 * 
 */
bool App::init()
{
	// �������V�[�h��ݒ�
	srand((unsigned int)time(NULL));

	// �T�E���h�̃C���X�^���X���擾
	g_pSound = Sound::getInstance();

	// �^�C�}�[��������
	if (!printInit(timerInit(), "Timer init"))
		return false;

	// �t�H���g��������
	if (!printInit(fontInit(), "Font init"))
		return false;

	// �T�E���h��������
	if (!printInit(g_pSound->init(), "Sound init"))
		return false;

	// �I�[�f�B�I��������
	if (!printInit(audioInit(), "Audio init"))
		return false;

	// �~���[�W�b�N��������
	if (!printInit(g_music.init(), "Music init"))
		return false;

	// �e�N�X�`���}�l�[�W���[��������
	if (!printInit(g_textureManager.init(), "Texture initAll"))
		return false;

	// �p�[�c�}�l�[�W���[��������
	if (!printInit(g_partManager.init(), "Part init"))
		return false;

	// �A�j���[�V������������
	if (!printInit(g_animations->initAll(), "Animation init"))
		return false;

	// �Q�[����������
	if (!printInit(m_pGame->init(), "Game init"))
		return false;

	return true;
}

/**
 * @breif �m�ۂ������\�[�X���������
 *
 * @param �Ȃ�
 *
 * @return �Ȃ�
 *
 */
void App::release()
{
	g_player.~Player();
	fontRelease();
	audioRelease();
	g_courseManager.release();
	g_textureManager.release();
	g_pSound->release();
	Mouse::release();
	Game::destroy();
}

/**
 * @breif �A�v���P�[�V�����̍X�V���s��
 *
 * @param �Ȃ�
 *
 * @return �Ȃ�
 *
 */
void App::update()
{
	// �I�[�f�B�I���X�V
	audioUpdate();
	Keyboard::begin();

	g_frameCounter.update();
	g_music.update();
	m_pGame->update();

	Keyboard::end();
}

/**
 * @breif �A�v���P�[�V�����̕`����s��
 *
 * @param �Ȃ�
 *
 * @return �Ȃ�
 *
 */
void App::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(
		g_courseManager.getScroll(), g_courseManager.getScroll() + (float)SCREEN_WIDTH,
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
		//	glLineWidth(f);// GLfloat width
		glPointSize(f);// GLfloat size
	}


	glColor3ub(0xff, 0xff, 0xff);

	m_pGame->draw();

	m_frames++;
	m_accumulator += m_deltaTime;

	if (m_accumulator > 0.5f) {
		float fps = m_frames / m_accumulator;
		m_frames = 0;
		m_accumulator = 0.0f;
	}

	glutSwapBuffers();
}

/**
 * @breif �A�v���P�[�V�����̃��C�����[�v���J�n����
 *
 * @param �Ȃ�
 *
 * @return �Ȃ�
 *
 */
void App::run()
{
	m_running = true;

	float accumlator = 0.0f;

	while (m_running) {
		glutMainLoopEvent();

		auto nowTime = std::chrono::steady_clock::now();
		m_deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(nowTime - m_currentTime).count() / 1000000.0f;
		m_deltaTime = std::min(m_deltaTime, 0.25f);
		m_currentTime = nowTime;

		accumlator += m_deltaTime;

		while (accumlator >= MS_PER_UPDATE) {
			update();
			accumlator -= MS_PER_UPDATE;
		}

		draw();
	}

	glutDestroyWindow(m_window);

	release();
}

/**
 * @breif �A�v���P�[�V���������
 *
 * @param �Ȃ�
 *
 * @return �Ȃ�
 *
 */
void App::close()
{
	m_running = false;
}

/**
 * @breif �������̌��ʂ��R���\�[���ɕ\������
 *
 * @param[in] _result	�������̌���
 * @param[in] _str		�\�����镶����
 *
 * @return ����: true ���s: false
 *
 */
bool App::printInit(int _result, const char* _str) const
{
	return printInit(_result == 0 ? true : false, _str);
}

/**
 * @breif �������̌��ʂ��R���\�[���ɕ\������
 *
 * @param[in] _result	�������̌���
 * @param[in] _str		�\�����镶����
 *
 * @return ����: true ���s: false
 *
 */
bool App::printInit(bool _result, const char* _str) const
{
	printf("[");
	if (_result) {
		SetConsoleTextAttribute(
			m_hConsoleOutput,	// HANDLE hConsoleOutput,
			FOREGROUND_GREEN);	// WORD wAttributes
		printf("  OK  ");
	} else {
		SetConsoleTextAttribute(
			m_hConsoleOutput,	// HANDLE hConsoleOutput,
			FOREGROUND_RED);	// WORD wAttributes
		printf("FAILED");
	}
	SetConsoleTextAttribute(
		m_hConsoleOutput,										// HANDLE hConsoleOutput,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);	// WORD wAttributes
	printf("] %s\n", _str);

	return _result;
}
