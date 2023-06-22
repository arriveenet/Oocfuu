#include "timer.h"
#include <Windows.h>

// �B��̃C���X�^���X
static Timer timer;

/*
 * @breif �^�C�}�[�����������s��
 * 
 * @param �Ȃ�
 * 
 * @return �����Ftrue ���s�Ffalse
 * 
 */
bool timerInit(void)
{
// �R���p�C���^�[�Q�b�g��Windows�ł͂Ȃ��ꍇfalse��Ԃ�
#ifndef _WIN32
	return false;
#endif // !_WIN32

	// �p�t�H�[�}���X �J�E���^�[�̕p�x���擾
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&timer.frequency) == FALSE)
		return false;

	// �I�t�Z�b�g���擾
	QueryPerformanceCounter((LARGE_INTEGER*)&timer.offset);

	return true;
}

/*
 * @breif �v���O�����J�n����̎��Ԃ��擾
 *
 * @param �Ȃ�
 *
 * @return �o�ߎ���
 *
 */
double getTime(void)
{
	return (double)(getTimerValue() - timer.offset) / timer.frequency;
}

/*
 * @breif �p�t�H�[�}���X�J�E���^�[�̌��݂̒l���擾
 *
 * @param �Ȃ�
 *
 * @return �p�t�H�[�}���X�J�E���^�[
 *
 */
uint64_t getTimerValue(void)
{
	uint64_t value = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&value);
	return value;
}

/*
 * @breif �p�t�H�[�}���X �J�E���^�[�̕p�x���擾
 *
 * @param �Ȃ�
 *
 * @return �p�t�H�[�}���X�J�E���^�[�̕p�x
 *
 */
uint64_t getTimerFrequency(void)
{
	return timer.frequency;
}
