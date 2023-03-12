#pragma once
#include "Part.h"
#include "common/common.h"

#include <glm/glm.hpp>

/** ヒットブロックを制御するクラス
*/ 
class HitBlockController {
	int m_count;			// カウント
	int m_part;				// 更新するパーツ
	int m_endPart;			// 更新後のパーツ
	glm::vec2 m_position;	// ブロックの位置
	glm::ivec2 m_point;		// コースの位置
	bool m_updated;			// 更新終了フラグ

public:
	// コンストラクタ
	HitBlockController();

	// デストラクタ
	virtual ~HitBlockController();

	// ブロックを上にずらす
	void start(const glm::ivec2& _point, const PART& _part, const PART& _endPart);

	// ブロックを更新
	bool update(QUAD& _quad);
};