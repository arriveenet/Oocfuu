#pragma once
#import <msxml6.dll> raw_interfaces_only

/* コース読み込みクラス
*/
class CourseLoader {
private:
	// デフォルトコンストラクタ
	CourseLoader();

public:
	// デストラクタ
	virtual ~CourseLoader();

	// インスタンスを作成
	static CourseLoader* create();
	
	// 初期化
	bool initialize(const char* pFileName);

private:
	MSXML2::IXMLDOMDocumentPtr m_pDocument;
};