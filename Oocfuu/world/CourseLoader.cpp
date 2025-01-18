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
 * @brief コンストラクタ
 * 
 */
CourseLoader::CourseLoader()
	: m_pRootElement(nullptr)
	, m_loadResult(Success)
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
	if (!_pFileName) {
		m_loadResult = OpenFileFailed;
		return false;
	}

	// ファイルを読み込む
	XMLError error =  m_document.LoadFile(_pFileName);

	// ファイル読み込み成功確認
	if (error != XML_SUCCESS) {
		m_loadResult = OpenFileFailed;
		return false;
	}

	// ルート要素を取得
	m_pRootElement = m_document.FirstChildElement();

	// ルート要素がNULLか確認する
	if (!m_pRootElement) {
		m_loadResult = InitializationFailed;
		return false;
	}

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
	// コースエフェクトをクリア
	CourseEffectManager::instance()->clear();
	g_enemyManager.clear();
	g_gmmickPart.clear();

	// ヘッダーを解析する
	XMLElement* pHeaderElement = m_pRootElement->FirstChildElement("header");
	assert(pHeaderElement);
	if (!parseHeader(_pCourse, pHeaderElement)) {
		m_loadResult = HeaderParseFailed;
		return false;
	}

	// コースデータを解析する
	XMLElement* pDataElement = m_pRootElement->FirstChildElement("data");
	assert(pDataElement);
	if (!parseCourse(_pCourse, pDataElement)) {
		return false;
	}

	// 仕掛けパーツを解析する
	XMLElement* pGimmickElement = m_pRootElement->FirstChildElement("gimmick");
	if (pGimmickElement) {
		if (!parseGimmickParts(&g_gmmickPart, pGimmickElement)) {
			m_loadResult = GimmickPartsParseFailed;
			return false;
		}
	}

	// 敵キャラクターを解析する
	XMLElement* pEnemyElement = m_pRootElement->FirstChildElement("enemy");
	if (pEnemyElement) {
		if (!parseEnemy(&g_enemyManager, pEnemyElement, _pCourse)) {
			m_loadResult = EnemyParseFailed;
			return false;
		}
	}

	// コースのオブジェクトを解析する
	XMLElement* pEntityElement = m_pRootElement->FirstChildElement("object");
	if (pEntityElement) {
		if (!parseObjects(_pCourse, pEntityElement)) {
			return false;
		}
	}

	// コースを作成
	_pCourse->create();

	// 読み込みフラグを設定
	_pCourse->m_isLoaded = true;

	// コースをインポート
	if (m_pCourseManager) {
		m_pCourseManager->import(_pCourse);
	}

	// XMLドキュメントをクリア
	m_document.Clear();

	return true;
}

CourseLoader::Result CourseLoader::getLastError()
{
	return m_loadResult;
}

/**
 * @brief コースのヘッダーを解析する
 *
 * @param[in] _pData			コースクラスのポインタ
 * @param[in] _pHeaderElement	ヘッダーのXML要素
 *
 * @return 成功：true 失敗：false
 *
 */
bool CourseLoader::parseHeader(Course* _pCourse, tinyxml2::XMLElement* _pHeaderElement)
{
	// コース情報の要素を取得
	XMLElement* element = _pHeaderElement->FirstChildElement("info");
	{
		// コースの幅を読み込む
		_pCourse->m_width = element->IntAttribute("width");

		// コースの高さを読み込む
		_pCourse->m_height = element->IntAttribute("height");
	}

	// コース種別の要素を取得
	element = _pHeaderElement->FirstChildElement("type");
	{
		_pCourse->setType(element->GetText());
	}

	// コースのクリアカラーの要素を取得
	element = _pHeaderElement->FirstChildElement("color");
	{
		_pCourse->m_clearColor = element->IntText();
	}

	// プレイヤーのスタート位置の要素を取得
	element = _pHeaderElement->FirstChildElement("start");
	{
		_pCourse->m_startPosition.x = element->FloatAttribute("x");
		_pCourse->m_startPosition.y = element->FloatAttribute("y");
	}

	// 次のコースの要素を取得
	element = _pHeaderElement->FirstChildElement("next");
	{
		_pCourse->m_nextWorld.world = element->IntAttribute("world");
		_pCourse->m_nextWorld.stage = element->IntAttribute("stage");
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

	// コースサイズが0以下の場合
	if ((courseWidth <= 0) || (courseHeight <= 0)) {
		m_loadResult = InvalidSize;
		return false;
	}

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
		m_loadResult = OutOfMemory;
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
				// 有効なパーツ判定
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
 * @brief 敵キャラクターを解析する
 *
 * @param[in] _pData			敵管理クラスのポインタ
 * @param[in] _pGimmickElement	仕掛けパーツのXML要素
 *
 * @return 成功：true 失敗：false
 *
 */
bool CourseLoader::parseEnemy(EnemyManager* _pEnemyManager, tinyxml2::XMLElement* _pEnemyElement, Course* _pCourse)
{
	assert(_pEnemyManager);

	auto EnemyFactory = _pEnemyManager->getFactory();

	// クリボーを読み込む
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

	// ノコノコを読み込む
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

	// 赤ノコノコを読み込む
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

	// クッパを読み込む
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
 * @brief コースの仕掛けパーツを解析する
 *
 * @param[in] _pData			コースクラスのポインタ
 * @param[in] _pGimmickElement	仕掛けパーツのXML要素
 *
 * @return 成功：true 失敗：false
 *
 */
bool CourseLoader::parseGimmickParts(GimmickPart* _pGimmickParts, tinyxml2::XMLElement* _pGimmickElement)
{
	assert(_pGimmickElement);

	// リフトを読み込む
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

	// ファイアバーを読み込む
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
 * @brief コースオブジェクトを解析する
 *
 * @param[in] _pCourse			コースクラスのポインタ
 * @param[in] _pObjectsElement	コースオブジェクトのXML要素
 *
 * @return 成功：true 失敗：false
 *
 */
bool CourseLoader::parseObjects(Course* _pCourse, tinyxml2::XMLElement* _pObjectsElement)
{
	assert(_pObjectsElement);

	// キノピオを読み込む
	tinyxml2::XMLElement* pKinopioElement = _pObjectsElement->FirstChildElement("kinopio");
	if (pKinopioElement) {
		for (tinyxml2::XMLElement* pElement = pKinopioElement; pElement != nullptr; pElement = pElement->NextSiblingElement()) {
			float x = pElement->FloatAttribute("x");
			float y = pElement->FloatAttribute("y");

			Kinopio* kinopio = new Kinopio(x, y);
			_pCourse->m_courseObjects.push_back(kinopio);
		}
	}

	// ケーキ読み込む
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
