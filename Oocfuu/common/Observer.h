#pragma once
#include "Entity.h"

/**
 * オブザーバー抽象クラス
 */
class Observer {
public:
	/**
	 * @brief デストラクタ
	 */
	virtual ~Observer() {};

	/**
	 * @brief 通知を受け取る
	 * 
	 * @param[in]	なし
	 * 
	 */
	virtual void onNotify() = 0;
};