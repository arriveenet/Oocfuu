#pragma once
#include "Game.h"
#include "Rect.h"
#include "common/common.h"
#include "Course.h"

#include <gl/glut.h>
#include <glm/glm.hpp>
#include <Windows.h>

#include <vector>
#include <string>

/**
* �R�[�X�Ǘ��N���X
*/
class CourseManager {
private:
	float					m_scroll;				//!< �X�N���[���ʒu
	bool					m_isLoaded;				//!< �R�[�X�ǂݍ��݃t���O
	Course					m_currentCourse;		//!< ���ݐݒ肳��Ă���R�[�X

public:
	// �R���X�g���N�^
	CourseManager();

	// �f�X�g���N�^
	virtual ~CourseManager();

	// �R�[�X�z������
	void release();

	// �R�[�X���N���A����
	void clear();

	// �R�[�X���X�V
	void update();

	// �R�[�X��`��
	void draw();

	// �X�N���[���ʒu��ݒ�
	void setScroll(const float _scroll) { m_scroll = _scroll; };

	// ���݂̃X�N���[�����擾
	float getScroll() const { return m_scroll; };

	// ���݂̃X�N���[���ʒu������Z����
	void addScroll(const float _scroll) { m_scroll += _scroll; };

	// �R�[�X�̕��܂ŃX�N���[��������
	bool isScrollMax();

	// �p�[�c��ݒ�
	void setParts(glm::ivec2 const& _point, int _parts);

	// �p�[�c���擾
	int getParts(int _x, int _y);

	// �p�[�c���擾
	int getParts(glm::vec2 const& _point);

	// ���݂̃R�[�X���擾
	Course& getCourse();

	// �R�[�X�Ƃ̓����蔻��
	bool intersect(glm::vec2 const& _point) const;

	// �R�[�X�̃R�C���Ƃ̓����蔻��
	void intersectCoin(Player* _pPlayer);

	// �u���b�N��@��
	void hitBlock(glm::vec2 const& _point);

	// �R�[�X�̕����擾
	int getWidth() const;

	// �R�[�X�̍������擾
	int getHeight() const { return m_currentCourse.m_height; }

	// �w�i�F���擾����
	COLORREF getClearColor() const { return m_currentCourse.m_clearColor; }

	// �v���C���[�̃X�^�[�g�ʒu���擾
	glm::ivec2 getStartPosition() const { return m_currentCourse.m_startPosition; }

	// ���̃��[���h���擾
	WORLD getNextWorld() const { return m_currentCourse.m_nextWorld; }

	// �R�[�X�̎�ʂ��擾
	Course::CourseType getCourseType() { return m_currentCourse.m_courseType; }

	// �N���A���Ɠ������Ă��邩
	bool getClearAex(Rect& _rect) const;

	// �{�X�X�e�[�W�̋���j�󂷂�
	void destroyBridge()
	{
		m_currentCourse.m_bridgeController.destroy();
	}

	// �{�X�X�e�[�W�̋����󂳂ꂽ��
	bool isBridgeDestroyed()
	{
		return m_currentCourse.m_bridgeController.isDestroyed();
	}

	// ��`����ʓ��ɂ��邩
	bool isInScreen(const Rect& _rect) const;

	// �R�[�X���C���|�[�g����
	void import(Course* pCourse);
};

extern CourseManager g_courseManager;