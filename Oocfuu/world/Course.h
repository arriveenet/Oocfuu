#pragma once
#include "Game.h"
#include "TextureManager.h"
#include "common/common.h"
#include "HitBlockController.h"
#include "BridgeController.h"

#include <Windows.h>
#include <glm/glm.hpp>
#include <string>

/**
 * @brief コースクラス
 * 
 */
class Course {
public:
	// コースの種別
	enum CourseType {
		Overworld,		//!< 地上
		Underground,	//!< 地下
		Castle,			//!< 城
		Max				//!< コース種別数
	};

	int			m_width;			//!< コースの幅
	int			m_height;			//!< コースの高さ
	int**		m_pParts;			//!< パーツの2次元配列
	CourseType	m_courseType;		//!< コースの種別
	COLORREF	m_clearColor;		//!< 背景色
	TEXTURE		m_texture;			//!< パーツのテクスチャ
	glm::vec2	m_startPosition;	//!< プレイヤーのスタート位置
	WORLD		m_nextWorld;		//!< 次のワールド
	bool		m_isLoaded;			//!< コース読み込みフラグ
	std::vector<QUAD>		m_quads;				//!< コースパーツの矩形
	std::vector<glm::ivec2>	m_coins;				//!< コインの位置
	Rect					m_clearAex;				//!< クリア斧
	HitBlockController		m_hitBlockController;	//!< 叩いたときのブロック制御
	BridgeController		m_bridgeController;		//!< ボスステージの橋制御


	// コンストラクタ
	Course();

	// デストラクタ
	virtual ~Course();

	// コースを作成
	void create();

	// コースを破棄
	void destroy();

	// コースを更新
	void update();

	// コースを描画
	void draw();

	// コースの幅を取得
	int getWidth() const noexcept;

	// コースの高さを取得
	int getHeight() const noexcept;

	// パーツのアドレスを取得
	int** getParts() const noexcept;

	// コースの種別を設定
	void setType(const std::string _typeName);
};

/**
 * @brief コースの幅を取得
 * 
 * @param なし
 * 
 * @return コースの幅
 * 
 */
inline 	int Course::getWidth() const noexcept
{
	return m_width;
}

/**
 * @brief コースの高さを取得
 *
 * @param なし
 *
 * @return コースの高さ
 *
 */
inline int Course::getHeight() const noexcept
{
	return m_height;
}

/**
 * @brief パーツの2次元配列のポインタを取得
 *
 * @param なし
 *
 * @return パーツの2次元配列
 *
 */
inline int** Course::getParts() const noexcept
{
	return m_pParts;
}
