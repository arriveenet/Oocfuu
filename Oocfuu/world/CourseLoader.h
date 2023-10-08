#pragma once
#include "Course.h"
#include "GimmickPart.h"
#include "CourseManager.h"
#include "enemy/EnemyManager.h"
#include "util/tinyxml2.h"

/**
 * @brief �R�[�X�ǂݍ��݂̌���
 */
enum class CourseLoadResult {
	Success = 0,				//!< ����
	InvalidValue,				//!< �����ȃp�[�c���w�肳��Ă���
	InvalidSize,				//!< �R�[�X�̃T�C�Y��0�ȉ�
	OutOfMemory,				//!< �R�[�X�f�[�^��ǂݍ��ރ��������c���Ă��Ȃ�
	OpenFileFailed,				//!< �R�[�X�t�@�C�����J���Ȃ�����
	InitializationFailed,		//!< �������Ɏ��s
	HeaderParseFailed,			//!< �w�b�_�[�̉�͂Ɏ��s
	EnemyParseFailed,			//!< �G�L�����N�^�[�̉�͂Ɏ��s
	GimmickPartsParseFailed,	//!< �d�|���p�[�c�̉�͂Ɏ��s
	Max
};

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
	bool parseHeader(Course* _pCourse, tinyxml2::XMLElement* _pHeaderElement);

	// �R�[�X����͂���
	bool parseCourse(Course* _pCourse, tinyxml2::XMLElement* _pDataElement);

	// �G�L�����N�^�[����͂���
	bool parseEnemy(EnemyManager* _pEnemyManager, tinyxml2::XMLElement * _pEnemyElement);

	// �d�|���p�[�c����͂���
	bool parseGimmickParts(GimmickPart* _pGimmickParts, tinyxml2::XMLElement* _pGimmickElement);

};