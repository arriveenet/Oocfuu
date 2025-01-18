#include "CourseLoader.h"

#include "Part.h"
#include "CourseEffect.h"
#include "CourseObject.h"

#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <cstdlib>

#define PART_STR_SIZE	2

using namespace tinyxml2;
using namespace glm;

CourseManager* CourseLoader::m_pCourseManager = nullptr;

/**
 * @brief �R���X�g���N�^
 * 
 */
CourseLoader::CourseLoader()
	: m_pRootElement(nullptr)
	, m_loadResult(Success)
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
	if (!_pFileName) {
		m_loadResult = OpenFileFailed;
		return false;
	}

	// �t�@�C����ǂݍ���
	XMLError error =  m_document.LoadFile(_pFileName);

	// �t�@�C���ǂݍ��ݐ����m�F
	if (error != XML_SUCCESS) {
		m_loadResult = OpenFileFailed;
		return false;
	}

	// ���[�g�v�f���擾
	m_pRootElement = m_document.FirstChildElement();

	// ���[�g�v�f��NULL���m�F����
	if (!m_pRootElement) {
		m_loadResult = InitializationFailed;
		return false;
	}

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
	// �R�[�X�G�t�F�N�g���N���A
	CourseEffectManager::instance()->clear();
	g_enemyManager.clear();
	g_gmmickPart.clear();

	// �w�b�_�[����͂���
	XMLElement* pHeaderElement = m_pRootElement->FirstChildElement("header");
	assert(pHeaderElement);
	if (!parseHeader(_pCourse, pHeaderElement)) {
		m_loadResult = HeaderParseFailed;
		return false;
	}

	// �R�[�X�f�[�^����͂���
	XMLElement* pDataElement = m_pRootElement->FirstChildElement("data");
	assert(pDataElement);
	if (!parseCourse(_pCourse, pDataElement)) {
		return false;
	}

	// �d�|���p�[�c����͂���
	XMLElement* pGimmickElement = m_pRootElement->FirstChildElement("gimmick");
	if (pGimmickElement) {
		if (!parseGimmickParts(&g_gmmickPart, pGimmickElement)) {
			m_loadResult = GimmickPartsParseFailed;
			return false;
		}
	}

	// �G�L�����N�^�[����͂���
	XMLElement* pEnemyElement = m_pRootElement->FirstChildElement("enemy");
	if (pEnemyElement) {
		if (!parseEnemy(&g_enemyManager, pEnemyElement, _pCourse)) {
			m_loadResult = EnemyParseFailed;
			return false;
		}
	}

	// �R�[�X�̃I�u�W�F�N�g����͂���
	XMLElement* pEntityElement = m_pRootElement->FirstChildElement("object");
	if (pEntityElement) {
		if (!parseObjects(_pCourse, pEntityElement)) {
			return false;
		}
	}

	// �R�[�X���쐬
	_pCourse->create();

	// �ǂݍ��݃t���O��ݒ�
	_pCourse->m_isLoaded = true;

	// �R�[�X���C���|�[�g
	if (m_pCourseManager) {
		m_pCourseManager->import(_pCourse);
	}

	// XML�h�L�������g���N���A
	m_document.Clear();

	return true;
}

CourseLoader::Result CourseLoader::getLastError()
{
	return m_loadResult;
}

/**
 * @brief �R�[�X�̃w�b�_�[����͂���
 *
 * @param[in] _pData			�R�[�X�N���X�̃|�C���^
 * @param[in] _pHeaderElement	�w�b�_�[��XML�v�f
 *
 * @return �����Ftrue ���s�Ffalse
 *
 */
bool CourseLoader::parseHeader(Course* _pCourse, tinyxml2::XMLElement* _pHeaderElement)
{
	// �R�[�X���̗v�f���擾
	XMLElement* element = _pHeaderElement->FirstChildElement("info");
	{
		// �R�[�X�̕���ǂݍ���
		_pCourse->m_width = element->IntAttribute("width");

		// �R�[�X�̍�����ǂݍ���
		_pCourse->m_height = element->IntAttribute("height");
	}

	// �R�[�X��ʂ̗v�f���擾
	element = _pHeaderElement->FirstChildElement("type");
	{
		_pCourse->setType(element->GetText());
	}

	// �R�[�X�̃N���A�J���[�̗v�f���擾
	element = _pHeaderElement->FirstChildElement("color");
	{
		_pCourse->m_clearColor = element->IntText();
	}

	// �v���C���[�̃X�^�[�g�ʒu�̗v�f���擾
	element = _pHeaderElement->FirstChildElement("start");
	{
		_pCourse->m_startPosition.x = element->FloatAttribute("x");
		_pCourse->m_startPosition.y = element->FloatAttribute("y");
	}

	// ���̃R�[�X�̗v�f���擾
	element = _pHeaderElement->FirstChildElement("next");
	{
		_pCourse->m_nextWorld.world = element->IntAttribute("world");
		_pCourse->m_nextWorld.stage = element->IntAttribute("stage");
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

	// �R�[�X�T�C�Y��0�ȉ��̏ꍇ
	if ((courseWidth <= 0) || (courseHeight <= 0)) {
		m_loadResult = InvalidSize;
		return false;
	}

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
		m_loadResult = OutOfMemory;
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
							_pCourse->m_clearAex.m_position = { j * PART_SIZE, i * PART_SIZE };
							break;
						case PART_BRIDGE:
							_pCourse->m_bridgeController.add(j, i);
							break;
						case PART_CHAIN:
							_pCourse->m_bridgeController.setChain(j, i);
							break;
						case PART_GOAL_TOP:
							CourseEffectManager::instance()->setGoalFlag(vec2(j * PART_SIZE - 8, i * PART_SIZE + 17));
							break;
						default:
							break;
						}

						break;
					}
				}
				// �L���ȃp�[�c����
				if (!validPart) {
					m_loadResult = InvalidValue;
					return false;
				}
			}
			index += PART_STR_SIZE;
		}
		pElement = pElement->NextSiblingElement();
		index = 0;
	}

	return true;
}

/**
 * @brief �G�L�����N�^�[����͂���
 *
 * @param[in] _pData			�G�Ǘ��N���X�̃|�C���^
 * @param[in] _pGimmickElement	�d�|���p�[�c��XML�v�f
 *
 * @return �����Ftrue ���s�Ffalse
 *
 */
bool CourseLoader::parseEnemy(EnemyManager* _pEnemyManager, tinyxml2::XMLElement* _pEnemyElement, Course* _pCourse)
{
	assert(_pEnemyManager);

	auto EnemyFactory = _pEnemyManager->getFactory();

	// �N���{�[��ǂݍ���
	tinyxml2::XMLElement* pKuriboElement = _pEnemyElement->FirstChildElement("kuribo");
	if (pKuriboElement) {
		for (tinyxml2::XMLElement* pElement = pKuriboElement;
			pElement != nullptr;
			pElement = pElement->NextSiblingElement("kuribo")) {
			Enemy* pEnemy = EnemyFactory->create("kuribo");
			float x = pElement->FloatAttribute("x");
			float y = pElement->FloatAttribute("y");
			pEnemy->setPosition(x, y);
			_pEnemyManager->addEnemy(pEnemy);
		}
	}

	// �m�R�m�R��ǂݍ���
	tinyxml2::XMLElement* pNokonokoElement = _pEnemyElement->FirstChildElement("nokonoko");
	if (pNokonokoElement) {
		for (tinyxml2::XMLElement* pElement = pNokonokoElement;
			pElement != nullptr;
			pElement = pElement->NextSiblingElement("nokonoko")) {
			Enemy* pEnemy = EnemyFactory->create("nokonoko");
			float x = pElement->FloatAttribute("x");
			float y = pElement->FloatAttribute("y");
			pEnemy->setPosition(x, y);
			_pEnemyManager->addEnemy(pEnemy);
		}
	}

	// �ԃm�R�m�R��ǂݍ���
	tinyxml2::XMLElement* pRedNokonokoElement = _pEnemyElement->FirstChildElement("red-nokonoko");
	if (pRedNokonokoElement) {
		for (tinyxml2::XMLElement* pElement = pRedNokonokoElement;
			pElement != nullptr;
			pElement = pElement->NextSiblingElement("red-nokonoko")) {
			Nokonoko* pEnemy = static_cast<Nokonoko*>(EnemyFactory->create("nokonoko"));
			float x = pElement->FloatAttribute("x");
			float y = pElement->FloatAttribute("y");
			pEnemy->setPosition(x, y);
			pEnemy->setType(Nokonoko::Type::Red);
			_pEnemyManager->addEnemy(pEnemy);
		}
	}

	// �N�b�p��ǂݍ���
	tinyxml2::XMLElement* pKoopaElement = _pEnemyElement->FirstChildElement("koopa");
	if (pKoopaElement) {
		for (tinyxml2::XMLElement* pElement = pKoopaElement;
			pElement != nullptr;
			pElement = pElement->NextSiblingElement("koopa")) {
			Enemy* pEnemy = EnemyFactory->create("koopa");
			float x = pElement->FloatAttribute("x");
			float y = pElement->FloatAttribute("y");
			pEnemy->setPosition(x, y);
			_pEnemyManager->addEnemy(pEnemy);

			RANGE range = _pCourse->m_bridgeController.getRange();
			g_enemyManager.setKoopaRange(range);
		}
	}

	return true;
}

/**
 * @brief �R�[�X�̎d�|���p�[�c����͂���
 *
 * @param[in] _pData			�R�[�X�N���X�̃|�C���^
 * @param[in] _pGimmickElement	�d�|���p�[�c��XML�v�f
 *
 * @return �����Ftrue ���s�Ffalse
 *
 */
bool CourseLoader::parseGimmickParts(GimmickPart* _pGimmickParts, tinyxml2::XMLElement* _pGimmickElement)
{
	assert(_pGimmickElement);

	// ���t�g��ǂݍ���
	tinyxml2::XMLElement* pLiftElement = _pGimmickElement->FirstChildElement("lift");
	if (pLiftElement) {
		for (tinyxml2::XMLElement* pElement = pLiftElement; pElement != nullptr; pElement = pElement->NextSiblingElement()) {
			float x = pElement->FloatAttribute("x");
			float y = pElement->FloatAttribute("y");
			int width = pElement->IntAttribute("width");
			int mode = pElement->IntAttribute("mode");

			_pGimmickParts->addLift(Lift(x, y, width, static_cast<LIFT_MOVEMENT>(mode)));
		}
	}

	// �t�@�C�A�o�[��ǂݍ���
	tinyxml2::XMLElement* pFirebarElement = _pGimmickElement->FirstChildElement("firebar");
	if (pFirebarElement) {
		for (tinyxml2::XMLElement* pElement = pFirebarElement; pElement != nullptr; pElement = pElement->NextSiblingElement()) {
			float x = pElement->FloatAttribute("x");
			float y = pElement->FloatAttribute("y");
			int rotate = pElement->IntAttribute("rotate");

			_pGimmickParts->addFirebar(Firebar(x, y, static_cast<FIREBAR_ROTATE>(rotate)));
		}
	}

	return true;
}

/**
 * @brief �R�[�X�I�u�W�F�N�g����͂���
 *
 * @param[in] _pCourse			�R�[�X�N���X�̃|�C���^
 * @param[in] _pObjectsElement	�R�[�X�I�u�W�F�N�g��XML�v�f
 *
 * @return �����Ftrue ���s�Ffalse
 *
 */
bool CourseLoader::parseObjects(Course* _pCourse, tinyxml2::XMLElement* _pObjectsElement)
{
	assert(_pObjectsElement);

	// �L�m�s�I��ǂݍ���
	tinyxml2::XMLElement* pKinopioElement = _pObjectsElement->FirstChildElement("kinopio");
	if (pKinopioElement) {
		for (tinyxml2::XMLElement* pElement = pKinopioElement; pElement != nullptr; pElement = pElement->NextSiblingElement()) {
			float x = pElement->FloatAttribute("x");
			float y = pElement->FloatAttribute("y");

			Kinopio* kinopio = new Kinopio(x, y);
			_pCourse->m_courseObjects.push_back(kinopio);
		}
	}

	// �P�[�L�ǂݍ���
	tinyxml2::XMLElement* pCakeElement = _pObjectsElement->FirstChildElement("cake");
	if (pCakeElement) {
		for (tinyxml2::XMLElement* pElement = pCakeElement; pElement != nullptr; pElement = pElement->NextSiblingElement()) {
			float x = pElement->FloatAttribute("x");
			float y = pElement->FloatAttribute("y");

			Cake* kinopio = new Cake(x, y);
			_pCourse->m_courseObjects.push_back(kinopio);
		}
	}
	return true;
}
