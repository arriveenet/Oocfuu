#pragma once
#include "Game.h"
#include "TextureManager.h"
#include "common/common.h"
#include "HitBlockController.h"
#include "BridgeController.h"

#include <Windows.h>
#include <glm/glm.hpp>
#include <string>

/**
 * @brief �R�[�X�N���X
 * 
 */
class Course {
public:
	// �R�[�X�̎��
	enum CourseType {
		Overworld,		//!< �n��
		Underground,	//!< �n��
		Castle,			//!< ��
		Max				//!< �R�[�X��ʐ�
	};

	int			m_width;			//!< �R�[�X�̕�
	int			m_height;			//!< �R�[�X�̍���
	int**		m_pParts;			//!< �p�[�c��2�����z��
	CourseType	m_courseType;		//!< �R�[�X�̎��
	COLORREF	m_clearColor;		//!< �w�i�F
	TEXTURE		m_texture;			//!< �p�[�c�̃e�N�X�`��
	glm::vec2	m_startPosition;	//!< �v���C���[�̃X�^�[�g�ʒu
	WORLD		m_nextWorld;		//!< ���̃��[���h
	bool		m_isLoaded;			//!< �R�[�X�ǂݍ��݃t���O
	std::vector<QUAD>		m_quads;				//!< �R�[�X�p�[�c�̋�`
	std::vector<glm::ivec2>	m_coins;				//!< �R�C���̈ʒu
	Rect					m_clearAex;				//!< �N���A��
	HitBlockController		m_hitBlockController;	//!< �@�����Ƃ��̃u���b�N����
	BridgeController		m_bridgeController;		//!< �{�X�X�e�[�W�̋�����


	// �R���X�g���N�^
	Course();

	// �f�X�g���N�^
	virtual ~Course();

	// �R�[�X���쐬
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

	// �R�[�X�̎�ʂ�ݒ�
	void setType(const std::string _typeName);
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

/**
 * @brief �p�[�c��2�����z��̃|�C���^���擾
 *
 * @param �Ȃ�
 *
 * @return �p�[�c��2�����z��
 *
 */
inline int** Course::getParts() const noexcept
{
	return m_pParts;
}
