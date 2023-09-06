#include "CourseLoader.h"

#include "Part.h"

#include <iostream>
#include <cstdlib>

#define PART_STR_SIZE	2

using namespace tinyxml2;

CourseManager* CourseLoader::m_pCourseManager = nullptr;

/**
 * @brief コンストラクタ
 * 
 */
CourseLoader::CourseLoader()
	: m_pRootElement(nullptr)
{
}

/**
 * @brief デストラクタ
 *
 */
CourseLoader::~CourseLoader()
{

}

/**
 * @brief コースローダーのインスタンスを取得
 * 
 * @param なし
 * 
 * @return コースローダーのポインタ
 *
 */
CourseLoader* CourseLoader::create(CourseManager* pManager)
{
	static CourseLoader instance;

	m_pCourseManager = pManager;

	return &instance;
}

/**
 * @brief コースローダーのインスタンスを破棄
 * 
 * @param なし
 * 
 * @return なし
 * 
 */
void CourseLoader::destroy()
{
	m_pCourseManager = nullptr;
}

/**
 * @brief コースローダーを初期化
 *
 * @param[in] _pFileName	ファイル名
 *
 * @return 成功：true 失敗：false
 *
 */
bool CourseLoader::initialize(const char* _pFileName)
{
	// ファイル名のポインタがNULLか確認する
	if (!_pFileName)
		return false;

	// ファイルを読み込む
	XMLError error =  m_document.LoadFile(_pFileName);

	// ファイル読み込み成功確認
	if (error != XML_SUCCESS)
		return false;

	// ルート要素を取得
	m_pRootElement = m_document.FirstChildElement();

	// ルート要素がNULLか確認する
	if (!m_pRootElement)
		return false;

	return true;
}

/**
 * @brief コースを読み込む
 *
 * @param[in] _pCourse	コースクラスのポインタ
 *
 * @return 成功：true 失敗：false
 *
 */
bool CourseLoader::load(Course* _pCourse)
{
	// ヘッダーを解析する
	XMLElement* pHeaderElement = m_pRootElement->FirstChildElement("header");
	assert(pHeaderElement);
	if (!parseHeader(_pCourse, pHeaderElement))
		return false;

	// コースデータを解析する
	XMLElement* pDataElement = m_pRootElement->FirstChildElement("data");
	assert(pDataElement);
	if (!parseCourse(_pCourse, pDataElement))
		return false;

	// 仕掛けパーツを解析する
	XMLElement* pGimmickElement = m_pRootElement->FirstChildElement("gimmick");
	if (pGimmickElement) {
		parseGimmickParts(&g_gmmickPart, pGimmickElement);
	}

	// コースを作成
	_pCourse->create();

	// 読み込みフラグを設定
	_pCourse->m_isLoaded = true;

	// コースをインポート
	if (m_pCourseManager) {
		m_pCourseManager->import(_pCourse);
	}

	return true;
}

/**
 * @brief コースのヘッダーを解析する
 *
 * @param[in] _pData		コースクラスのポインタ
 * @param[in] _pDataElement	ヘッダーのXML要素
 *
 * @return 成功：true 失敗：false
 *
 */
bool CourseLoader::parseHeader(Course* _pCourse, tinyxml2::XMLElement* _pDataElement)
{
	// コース情報の要素を取得
	XMLElement* element = _pDataElement->FirstChildElement("info");
	{
		// コースの幅を読み込む
		const char* attribute = element->Attribute("width");
		_pCourse->m_width = std::atoi(attribute);

		// コースの高さを読み込む
		attribute = element->Attribute("height");
		_pCourse->m_height = std::atoi(attribute);
	}

	// コースのクリアカラーの要素を取得
	element = _pDataElement->FirstChildElement("color");
	{
		const char* color = element->GetText();
		_pCourse->m_clearColor = std::atoi(color);
	}

	// プレイヤーのスタート位置の要素を取得
	element = _pDataElement->FirstChildElement("start");
	{
		char* stopstring;
		const char* attribute = element->Attribute("x");
		_pCourse->m_startPosition.x = std::strtof(attribute, &stopstring);

		attribute = element->Attribute("y");
		_pCourse->m_startPosition.y = std::strtof(attribute, &stopstring);
	}

	// 次のコースの要素を取得
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
 * @brief コースのパーツを解析する
 *
 * @param[in] _pData		コースクラスのポインタ
 * @param[in] _pDataElement	コースのXML要素
 *
 * @return 成功：true 失敗：false
 *
 */
bool CourseLoader::parseCourse(Course* _pCourse, tinyxml2::XMLElement* _pDataElement)
{
	const int courseWidth = _pCourse->m_width;
	const int courseHeight = _pCourse->m_height;
	assert(courseWidth > 0);
	assert(courseHeight > 0);

	// NULLでなければメモリを解放する
	if (_pCourse->m_pParts) {
		for (int i = 0; i < courseHeight; ++i) {
			delete _pCourse->m_pParts[i];
		}
		delete[] _pCourse->m_pParts;
	}

	// メモリを確保
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
			// すべてスペース(パーツなし)の場合nullptrが返ってくる
			// PART_NONEを設定し次の行へ
			if (!text) {
				std::memset(_pCourse->m_pParts[i], PART_NONE, sizeof(int) * courseWidth);
				break;
			}

			char buf[2] = { 0 };
			std::memcpy(buf, text + index, 2);

			//printf("[%d-%d] %c%c\n", i, j, buf[0], buf[1]);
			// パーツが何もない場合
			if (buf[0] == 0x20) {
				_pCourse->m_pParts[i][j] = PART_NONE;

			// パーツがある場合
			} else {
				bool validPart = false;	// 有効なパーツフラグ
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
				// 有効なパーツ判定
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
