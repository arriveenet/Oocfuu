#include "CourseLoader.h"
#include <iostream>
#include <cstdlib>

using namespace tinyxml2;

/**
 * @brief �R���X�g���N�^
 * 
 */
CourseLoader::CourseLoader()
{
}

/**
 * @brief �f�X�g���N�^
 *
 */
CourseLoader::~CourseLoader()
{

}

/**
 * @brief �R�[�X���[�_�[�̃C���X�^���X���擾
 * 
 * @param �Ȃ�
 * 
 * @return �R�[�X���[�_�[�̃|�C���^
 *
 */
CourseLoader* CourseLoader::create()
{
	static CourseLoader instance;
	return &instance;
}

/**
 * @brief �R�[�X���[�_�[��������
 *
 * @param[in] _pFileName	�t�@�C����
 *
 * @return �����Ftrue ���s�Ffalse
 *
 */
bool CourseLoader::initialize(const char* _pFileName)
{
	// �t�@�C�����̃|�C���^��NULL���m�F����
	if (!_pFileName)
		return false;

	// �t�@�C����ǂݍ���
	XMLError error =  m_document.LoadFile(_pFileName);

	// �t�@�C���ǂݍ��ݐ����m�F
	if (error != XML_SUCCESS)
		return false;

	// ���[�g�v�f���擾
	m_pRootElement = m_document.FirstChildElement();

	// ���[�g�v�f��NULL���m�F����
	if (!m_pRootElement)
		return false;

	return true;
}

/**
 * @brief �R�[�X��ǂݍ���
 *
 * @param[in] _pCourse	�R�[�X�N���X�̃|�C���^
 *
 * @return �����Ftrue ���s�Ffalse
 *
 */
bool CourseLoader::load(Course* _pCourse)
{

	XMLElement* headerElement = m_pRootElement->FirstChildElement("header");

	XMLElement* infoElement = headerElement->FirstChildElement();

	const char* attribute = infoElement->Attribute("width");
	_pCourse->m_width = std::atoi(attribute);

	attribute = infoElement->Attribute("height");
	_pCourse->m_height = std::atoi(attribute);

	XMLElement* dataElement = m_pRootElement->FirstChildElement("data");

	if (!parseCourse(dataElement->GetText()))
		return false;


	return true;
}

/**
 * @brief �R�[�X�̃p�[�c����͂���
 *
 * @param[in] _pData	�R�[�X�N���X�̃|�C���^
 *
 * @return �����Ftrue ���s�Ffalse
 *
 */
bool CourseLoader::parseCourse(const char* _pData)
{
	return true;
}
