#include "CourseLoader.h"
#include <iostream>
#include <cstdlib>

using namespace tinyxml2;

/**
 * @brief コンストラクタ
 * 
 */
CourseLoader::CourseLoader()
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
CourseLoader* CourseLoader::create()
{
	static CourseLoader instance;
	return &instance;
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
 * @brief コースのパーツを解析する
 *
 * @param[in] _pData	コースクラスのポインタ
 *
 * @return 成功：true 失敗：false
 *
 */
bool CourseLoader::parseCourse(const char* _pData)
{
	return true;
}
