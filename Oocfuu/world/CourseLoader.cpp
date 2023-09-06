#include "CourseLoader.h"

#include "Part.h"

#include <iostream>
#include <cstdlib>

#define PART_STR_SIZE	2

using namespace tinyxml2;

CourseManager* CourseLoader::m_pCourseManager = nullptr;

/**
 * @brief �R���X�g���N�^
 * 
 */
CourseLoader::CourseLoader()
	: m_pRootElement(nullptr)
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
CourseLoader* CourseLoader::create(CourseManager* pManager)
{
	static CourseLoader instance;

	m_pCourseManager = pManager;

	return &instance;
}

/**
 * @brief �R�[�X���[�_�[�̃C���X�^���X��j��
 * 
 * @param �Ȃ�
 * 
 * @return �Ȃ�
 * 
 */
void CourseLoader::destroy()
{
	m_pCourseManager = nullptr;
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
	// �w�b�_�[����͂���
	XMLElement* pHeaderElement = m_pRootElement->FirstChildElement("header");
	assert(pHeaderElement);
	if (!parseHeader(_pCourse, pHeaderElement))
		return false;

	// �R�[�X�f�[�^����͂���
	XMLElement* pDataElement = m_pRootElement->FirstChildElement("data");
	assert(pDataElement);
	if (!parseCourse(_pCourse, pDataElement))
		return false;

	// �d�|���p�[�c����͂���
	XMLElement* pGimmickElement = m_pRootElement->FirstChildElement("gimmick");
	if (pGimmickElement) {
		parseGimmickParts(&g_gmmickPart, pGimmickElement);
	}

	// �R�[�X���쐬
	_pCourse->create();

	// �ǂݍ��݃t���O��ݒ�
	_pCourse->m_isLoaded = true;

	// �R�[�X���C���|�[�g
	if (m_pCourseManager) {
		m_pCourseManager->import(_pCourse);
	}

	return true;
}

/**
 * @brief �R�[�X�̃w�b�_�[����͂���
 *
 * @param[in] _pData		�R�[�X�N���X�̃|�C���^
 * @param[in] _pDataElement	�w�b�_�[��XML�v�f
 *
 * @return �����Ftrue ���s�Ffalse
 *
 */
bool CourseLoader::parseHeader(Course* _pCourse, tinyxml2::XMLElement* _pDataElement)
{
	// �R�[�X���̗v�f���擾
	XMLElement* element = _pDataElement->FirstChildElement("info");
	{
		// �R�[�X�̕���ǂݍ���
		const char* attribute = element->Attribute("width");
		_pCourse->m_width = std::atoi(attribute);

		// �R�[�X�̍�����ǂݍ���
		attribute = element->Attribute("height");
		_pCourse->m_height = std::atoi(attribute);
	}

	// �R�[�X�̃N���A�J���[�̗v�f���擾
	element = _pDataElement->FirstChildElement("color");
	{
		const char* color = element->GetText();
		_pCourse->m_clearColor = std::atoi(color);
	}

	// �v���C���[�̃X�^�[�g�ʒu�̗v�f���擾
	element = _pDataElement->FirstChildElement("start");
	{
		char* stopstring;
		const char* attribute = element->Attribute("x");
		_pCourse->m_startPosition.x = std::strtof(attribute, &stopstring);

		attribute = element->Attribute("y");
		_pCourse->m_startPosition.y = std::strtof(attribute, &stopstring);
	}

	// ���̃R�[�X�̗v�f���擾
	element = _pDataElement->FirstChildElement("next");
	{
		const char* attribute = element->Attribute("world");
		_pCourse->m_nextWorld.world = std::atoi(attribute);

		attribute = element->Attribute("stage");
		_pCourse->m_nextWorld.stage = std::atoi(attribute);
	}

	return true;
}

/**
 * @brief �R�[�X�̃p�[�c����͂���
 *
 * @param[in] _pData		�R�[�X�N���X�̃|�C���^
 * @param[in] _pDataElement	�R�[�X��XML�v�f
 *
 * @return �����Ftrue ���s�Ffalse
 *
 */
bool CourseLoader::parseCourse(Course* _pCourse, tinyxml2::XMLElement* _pDataElement)
{
	const int courseWidth = _pCourse->m_width;
	const int courseHeight = _pCourse->m_height;
	assert(courseWidth > 0);
	assert(courseHeight > 0);

	// NULL�łȂ���΃��������������
	if (_pCourse->m_pParts) {
		for (int i = 0; i < courseHeight; ++i) {
			delete _pCourse->m_pParts[i];
		}
		delete[] _pCourse->m_pParts;
	}

	// ���������m��
	try {
		_pCourse->m_pParts = new int* [_pCourse->m_height];
		for (int i = 0; i < _pCourse->m_height; i++) {
			_pCourse->m_pParts[i] = new int[_pCourse->m_width] {PART_NONE};
		}
	} catch (std::bad_alloc& ex) {
		std::cerr << "CourseLoader::parseCourse: " << ex.what() << std::endl;
		return false;
	}

	int index = 0;
	tinyxml2::XMLElement* pElement = _pDataElement->FirstChildElement("dr");

	for (int i = 0; i < courseHeight; i++) {
		const char* text = pElement->GetText();

		for (int j = 0; j < courseWidth; j++) {
			// ���ׂăX�y�[�X(�p�[�c�Ȃ�)�̏ꍇnullptr���Ԃ��Ă���
			// PART_NONE��ݒ肵���̍s��
			if (!text) {
				std::memset(_pCourse->m_pParts[i], PART_NONE, sizeof(int) * courseWidth);
				break;
			}

			char buf[2] = { 0 };
			std::memcpy(buf, text + index, 2);

			//printf("[%d-%d] %c%c\n", i, j, buf[0], buf[1]);
			// �p�[�c�������Ȃ��ꍇ
			if (buf[0] == 0x20) {
				_pCourse->m_pParts[i][j] = PART_NONE;

			// �p�[�c������ꍇ
			} else {
				bool validPart = false;	// �L���ȃp�[�c�t���O
				for (int k = PART_NONE + 1; k < PART_MAX; k++) {
					if (strncmp(buf, g_parts[k].m_fileName, 2) == 0) {
						validPart = true;
						_pCourse->m_pParts[i][j] = k;

						switch (k) {
						case PART_AXE_0:
							//m_clearAex.m_position = { j * PART_SIZE, i * PART_SIZE };
							break;
						case PART_BRIDGE:
							//m_bridgeController.add(j, i);
							break;
						case PART_CHAIN:
							//m_bridgeController.setChain(j, i);
							break;
						case PART_GOAL_TOP:
							//courseEffectMgr->setGoalFlag(vec2(j * PART_SIZE - 8, i * PART_SIZE + 17));
							break;
						default:
							break;
						}

						break;
					}
				}
				// �L���ȃp�[�c����
				if (!validPart) {
					return false;
				}
			}
			index += 2;
		}
		pElement = pElement->NextSiblingElement();
		index = 0;
	}

	return true;
}

bool CourseLoader::parseGimmickParts(GimmickPart* _pGimmickParts, tinyxml2::XMLElement* _pDataElement)
{
	assert(_pDataElement);

	tinyxml2::XMLElement* pLiftElement = _pDataElement->FirstChildElement("lift");
	if (pLiftElement) {
		for (tinyxml2::XMLElement* pElement = pLiftElement; pElement != NULL; pElement = pElement->NextSiblingElement()) {
			float x = std::atof(pElement->Attribute("x"));
			float y = std::atof( pElement->Attribute("y"));
			int width = std::atoi(pElement->Attribute("width"));
			int mode = std::atoi(pElement->Attribute("mode"));

			_pGimmickParts->addLift(Lift(x, y, width, static_cast<LIFT_MOVEMENT>(mode)));
		}
	}

	return true;
}
