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

/** �q�b�g�u���b�N�𐧌䂷��N���X
*/ 
class HitBlockController {
	int m_count;			// �J�E���g
	int m_part;				// �X�V����p�[�c
	int m_endPart;			// �X�V��̃p�[�c
	glm::vec2 m_position;	// �u���b�N�̈ʒu
	glm::ivec2 m_point;		// �R�[�X�̈ʒu
	bool m_updated;			// �X�V�I���t���O

public:
	// �R���X�g���N�^
	HitBlockController();

	// �f�X�g���N�^
	virtual ~HitBlockController();

	// �u���b�N����ɂ��炷
	void start(const glm::ivec2& _point, const PART& _part, const PART& _endPart);

	// �u���b�N���X�V
	bool update(QUAD& _quad);
};