#pragma once
#include "Course.h"
#include "CourseManager.h"
#include "util/tinyxml2.h"

/**
 * @brief �R�[�X�ǂݍ��݃N���X
 */
class CourseLoader {
private:
	tinyxml2::XMLDocument m_document;		//!< XML�h�L�������g
	tinyxml2::XMLElement* m_pRootElement;	//!< ���[�g�v�f
	static CourseManager* m_pCourseManager;	//!< �R�[�X�}�l�[�W���[

	// �f�t�H���g�R���X�g���N�^
	CourseLoader();

public:
	// �f�X�g���N�^
	virtual ~CourseLoader();

	// �C���X�^���X���쐬
	static CourseLoader* create(CourseManager* pManager);

	// �C���X�^���X��j��
	void destroy();
	
	// ������
	bool initialize(const char* _pFileName);

	// �R�[�X��ǂݍ���
	bool load(Course* _pCourse);

private:
	// �w�b�_�[����͂���
	bool parseHeader(Course* _pCourse, tinyxml2::XMLElement* _pDataElement);

	// �R�[�X����͂���
	bool parseCourse(Course* _pCourse, tinyxml2::XMLElement* _pDataElement);

};