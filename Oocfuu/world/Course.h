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
	int getWidth() const noexcept { return m_width; }

	// �R�[�X�̍������擾
	int getHeight() const noexcept { return m_height; }

	// �p�[�c�̃A�h���X���擾
	int** getParts() const noexcept { return m_pParts; }

	// �R�[�X�̎�ʂ�ݒ�
	void setType(const std::string _typeName);

	const std::vector<Sprite*>& getCourceObject() const { return m_courseObjects; }

	int getPartIndexByPos(int x, int y) const { return x + y * m_width; }

	void setPart(int x, int y, int part);

protected:
	void updateTotalQuads();
	void updateParts();

protected:
	int			m_width;			//!< �R�[�X�̕�
	int			m_height;			//!< �R�[�X�̍���
	int**		m_pParts;			//!< �p�[�c��2�����z��
	CourseType	m_courseType;		//!< �R�[�X�̎��
	COLORREF	m_clearColor;		//!< �w�i�F
	TEXTURE		m_texture;			//!< �p�[�c�̃e�N�X�`��
	glm::vec2	m_startPosition;	//!< �v���C���[�̃X�^�[�g�ʒu
	WORLD		m_nextWorld;		//!< ���̃��[���h
	bool		m_isLoaded;			//!< �R�[�X�ǂݍ��݃t���O
	bool		m_quadsDirty;
	std::vector<QUAD>			m_quads;				//!< �R�[�X�p�[�c�̋�`
	std::vector<unsigned short> m_indices;
	std::vector<int>			m_partToQuadIndex;
	std::vector<glm::ivec2>		m_coins;				//!< �R�C���̈ʒu
	Rect						m_clearAex;				//!< �N���A��
	HitBlockController			m_hitBlockController;	//!< �@�����Ƃ��̃u���b�N����
	BridgeController			m_bridgeController;		//!< �{�X�X�e�[�W�̋�����
	std::vector<Sprite*>		m_courseObjects;

	friend class CourseLoader;
	friend class CourseManager;
};
