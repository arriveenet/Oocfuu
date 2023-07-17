#pragma once

/**
 * @brief エンティティ抽象クラス
 * 
 */
struct Entity {
	/**
	 * @brief 更新処理
	 */
	virtual void update() = 0;

	/**
	 * @brief 描画処理
	 */
	virtual void draw() = 0;
};