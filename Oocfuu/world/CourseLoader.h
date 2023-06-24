#pragma once
#include "Course.h"
#include "util/tinyxml2.h"

class Course;

/**
 * @brief コース読み込みクラス
 */
class CourseLoader {
private:
	tinyxml2::XMLDocument m_document;	//!< XMLドキュメント
	tinyxml2::XMLElement* m_pRootElement;	//!< ルート要素

	// デフォルトコンストラクタ
	CourseLoader();

public:
	// デストラクタ
	virtual ~CourseLoader();

	// インスタンスを作成
	static CourseLoader* create();
	
	// 初期化
	bool initialize(const char* _pFileName);

	bool load(Course* _pCourse);

private:
	// コースを解析する
	bool parseCourse(const char* _pData);

};