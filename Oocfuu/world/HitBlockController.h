#pragma once
#include "Part.h"
#include "common/common.h"

#include <glm/glm.hpp>

/** �q�b�g�u���b�N�𐧌䂷��N���X
*/ 
class HitBlockController {
	int m_count;			// �J�E���g
	int m_part;				// �X�V����p�[�c
	int m_endPart;			// �X�V��̃p�[�c
	glm::vec2 m_position;	// �u���b�N�̈ʒu
	glm::ivec2 m_point;		// �R�[�X�̈ʒu
	bool m_updated;			// �X�V�I���t���O

public:
	// �R���X�g���N�^
	HitBlockController();

	// �f�X�g���N�^
	virtual ~HitBlockController();

	// �u���b�N����ɂ��炷
	void start(const glm::ivec2& _point, const PART& _part, const PART& _endPart);

	// �u���b�N���X�V
	bool update(QUAD& _quad);
};