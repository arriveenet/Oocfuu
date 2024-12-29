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
	int getWidth() const noexcept { return m_width; }

	// コースの高さを取得
	int getHeight() const noexcept { return m_height; }

	// パーツのアドレスを取得
	int** getParts() const noexcept { return m_pParts; }

	// コースの種別を設定
	void setType(const std::string _typeName);

	const std::vector<Sprite*>& getCourceObject() const { return m_courseObjects; }

	int getPartIndexByPos(int x, int y) const { return x + y * m_width; }

	void setPart(int x, int y, int part);

protected:
	void updateTotalQuads();
	void updateParts();

protected:
	int			m_width;			//!< コースの幅
	int			m_height;			//!< コースの高さ
	int**		m_pParts;			//!< パーツの2次元配列
	CourseType	m_courseType;		//!< コースの種別
	COLORREF	m_clearColor;		//!< 背景色
	TEXTURE		m_texture;			//!< パーツのテクスチャ
	glm::vec2	m_startPosition;	//!< プレイヤーのスタート位置
	WORLD		m_nextWorld;		//!< 次のワールド
	bool		m_isLoaded;			//!< コース読み込みフラグ
	bool		m_quadsDirty;
	std::vector<QUAD>			m_quads;				//!< コースパーツの矩形
	std::vector<unsigned short> m_indices;
	std::vector<int>			m_partToQuadIndex;
	std::vector<glm::ivec2>		m_coins;				//!< コインの位置
	Rect						m_clearAex;				//!< クリア斧
	HitBlockController			m_hitBlockController;	//!< 叩いたときのブロック制御
	BridgeController			m_bridgeController;		//!< ボスステージの橋制御
	std::vector<Sprite*>		m_courseObjects;

	friend class CourseLoader;
	friend class CourseManager;
};
