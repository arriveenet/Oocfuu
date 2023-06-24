#pragma once
#include "Course.h"
#include "util/tinyxml2.h"

class Course;

/**
 * @brief �R�[�X�ǂݍ��݃N���X
 */
class CourseLoader {
private:
	tinyxml2::XMLDocument m_document;	//!< XML�h�L�������g
	tinyxml2::XMLElement* m_pRootElement;	//!< ���[�g�v�f

	// �f�t�H���g�R���X�g���N�^
	CourseLoader();

public:
	// �f�X�g���N�^
	virtual ~CourseLoader();

	// �C���X�^���X���쐬
	static CourseLoader* create();
	
	// ������
	bool initialize(const char* _pFileName);

	bool load(Course* _pCourse);

private:
	// �R�[�X����͂���
	bool parseCourse(const char* _pData);

};