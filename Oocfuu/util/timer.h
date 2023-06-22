#pragma once
#include <stdint.h>

/**
 * @brief �^�C�}�[�\����
 */
struct Timer {
	uint64_t frequency;
	uint64_t offset;
};

// �^�C�}�[��������
bool timerInit(void);

// �v���O�����J�n����̎��Ԃ��擾
double getTime(void);

// �p�t�H�[�}���X�J�E���^�[�̌��݂̒l���擾
uint64_t getTimerValue(void);

// �p�t�H�[�}���X �J�E���^�[�̕p�x���擾
uint64_t getTimerFrequency(void);