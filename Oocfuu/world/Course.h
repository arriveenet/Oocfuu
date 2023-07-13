#pragma once
#include <Windows.h>
#include <glm/glm.hpp>
#include "Game.h"
#include "CourseManager.h"
#include "CourseLoader.h"

/**
 * @breif �R�[�X�N���X
 * 
 */
class Course {
public:
	int			m_width;			//!< �R�[�X�̕�
	int			m_height;			//!< �R�[�X�̍���
	int**		m_pParts;			//!< �p�[�c��2�����z��
	COLORREF	m_clearColor;		//!< �w�i�F
	glm::vec2	m_startPosition;	//!< �v���C���[�̃X�^�[�g�ʒu
	WORLD		m_nexstWorld;		//!< ���̃��[���h


	// �R���X�g���N�^
	Course();

	// �f�X�g���N�^
	virtual ~Course();

	void clear();

	// �R�[�X�̕����擾
	int getWidth() const noexcept;

	// �R�[�X�̍������擾
	int getHeight() const noexcept;

	int** getParts() const noexcept;
};

/**
 * @brief �R�[�X�̕����擾
 * 
 * @param �Ȃ�
 * 
 * @return �R�[�X�̕�
 * 
 */
inline 	int Course::getWidth() const noexcept
{
	return m_width;
}

/**
 * @brief �R�[�X�̍������擾
 *
 * @param �Ȃ�
 *
 * @return �R�[�X�̍���
 *
 */
inline int Course::getHeight() const noexcept
{
	return m_height;
}

inline int** Course::getParts() const noexcept
{
	return m_pParts;
}
