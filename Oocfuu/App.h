#pragma once
#include <time.h>
#include <Windows.h>
#include "common/common.h"

/**
 * @brief �A�v���P�[�V�����N���X
 */
class App {
private:
	HANDLE m_hConsoleOutput;	//!< �R���\�[���n���h��
	bool m_running;				//!< ���C�����[�v�t���O
	int m_window;				//!< �E�B���h�E�n���h��

public:
	// �R���X�g���N�^
	App();
	// �f�X�g���N�^
	virtual ~App();

	// �A�v���P�[�V����������������
	bool init();

	// �A�v���P�[�V�����̃��\�[�X���������
	void release();

	// �A�v���P�[�V�����̍X�V����
	void update();

	// �A�v���P�[�V�����̕`�揈��
	void draw();

	// �A�v���P�[�V�����̃��C�����[�v���J�n
	void run();

	// �A�v���P�[�V���������
	void close();

	// �E�B���h�E��ݒ肷��
	void setWindow(int _window) noexcept { m_window = _window; }
	
protected:
	// �������̌��ʂ��R���\�[���ɕ\������
	bool printInit(int _result, const char* _str);
	bool printInit(bool _result, const char* _str);
};

extern App g_app;
