#pragma once
#import <msxml6.dll> raw_interfaces_only

/* �R�[�X�ǂݍ��݃N���X
*/
class CourseLoader {
private:
	// �f�t�H���g�R���X�g���N�^
	CourseLoader();

public:
	// �f�X�g���N�^
	virtual ~CourseLoader();

	// �C���X�^���X���쐬
	static CourseLoader* create();
	
	// ������
	bool initialize(const char* pFileName);

private:
	MSXML2::IXMLDOMDocumentPtr m_pDocument;
};