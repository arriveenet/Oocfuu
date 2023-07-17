#pragma once
#include "Game.h"
#include "common/common.h"

#include <Windows.h>
#include <glm/glm.hpp>

/**
 * @brief �R�[�X�N���X
 * 
 */
class Course {
public:
	int			m_width;			//!< �R�[�X�̕�
	int			m_height;			//!< �R�[�X�̍���
	int**		m_pParts;			//!< �p�[�c��2�����z��
	COLORREF	m_clearColor;		//!< �w�i�F
	glm::vec2	m_startPosition;	//!< �v���C���[�̃X�^�[�g�ʒu
	WORLD		m_nextWorld;		//!< ���̃��[���h
	bool		m_isLoaded;			// �R�[�X�ǂݍ��݃t���O
	std::vector<QUAD> m_quads;		// �R�[�X�p�[�c�̋�`


	// �R���X�g���N�^
	Course();

	// �f�X�g���N�^
	virtual ~Course();

	// �R�[�X���N���A
	void create();

	// �R�[�X��j��
	void destroy();

	// �R�[�X���X�V
	void update();

	// �R�[�X��`��
	void draw();

	// �R�[�X�̕����擾
	int getWidth() const noexcept;

	// �R�[�X�̍������擾
	int getHeight() const noexcept;

	// �p�[�c�̃A�h���X���擾
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
