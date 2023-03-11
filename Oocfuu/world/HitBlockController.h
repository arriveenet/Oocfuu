#pragma once
#include "Rect.h"
#include "Part.h"
#include "common/common.h"
#include "animation/AnimationController.h"

#include <glm/glm.hpp>
#include <vector>

#define BLOCK_COIN_WIDTH	8
#define BLOCK_COIN_HEIGHT	16
#define BLOCK_COIN_TTL		24

class BlockCoin : Rect {
private:
	uint8_t m_count;
	const uint8_t m_ttl;
	bool m_updated;
	AnimationController m_animationController;

public:
	BlockCoin(const glm::vec2& _position, uint8_t _ttl);
	virtual ~BlockCoin();

	void update();
	void draw() override;
	bool isUpdated() { return m_updated; };
};

class BlockCoinManager {
private:
	BlockCoinManager() {};
	BlockCoinManager(const BlockCoinManager&) {};

	std::vector<BlockCoin> m_blockCoins;
public:
	virtual ~BlockCoinManager();

	static BlockCoinManager* instance();

	void add(const BlockCoin& _blockCoin);
	void update();
	void draw();
};

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