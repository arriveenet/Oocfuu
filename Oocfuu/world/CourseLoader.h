#pragma once
#include "Course.h"
#include "GimmickPart.h"
#include "CourseManager.h"
#include "enemy/EnemyManager.h"
#include "util/tinyxml2.h"

/**
 * @brief コース読み込みクラス
 */
class CourseLoader {
private:
	tinyxml2::XMLDocument m_document;		//!< XMLドキュメント
	tinyxml2::XMLElement* m_pRootElement;	//!< ルート要素
	static CourseManager* m_pCourseManager;	//!< コースマネージャー

	// デフォルトコンストラクタ
	CourseLoader();

public:
	// デストラクタ
	virtual ~CourseLoader();

	// インスタンスを作成
	static CourseLoader* create(CourseManager* pManager);

	// インスタンスを破棄
	void destroy();
	
	// 初期化
	bool initialize(const char* _pFileName);

	// コースを読み込む
	bool load(Course* _pCourse);

private:
	// ヘッダーを解析する
	bool parseHeader(Course* _pCourse, tinyxml2::XMLElement* _pHeaderElement);

	// コースを解析する
	bool parseCourse(Course* _pCourse, tinyxml2::XMLElement* _pDataElement);

	// 敵キャラクターを解析する
	bool parseEnemy(EnemyManager* _pEnemyManager, tinyxml2::XMLElement * _pEnemyElement);

	// 仕掛けパーツを解析する
	bool parseGimmickParts(GimmickPart* _pGimmickParts, tinyxml2::XMLElement* _pGimmickElement);

};