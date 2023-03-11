#include "HitBlockController.h"
#include "TextureManager.h"
#include "Course.h"
#include "animation/Animation.h"

#include <gl/glut.h>

using namespace glm;
using namespace std;

static const char coinEffectTable[] ={ -2,-3,-4, -4, -4, -4, -4, -4, -3, -3, -2, -2, -1, 0, 0,
										0, 0, 1, 2, 2, 3, 3, 4, 4, 4, 4, 4, 4, 3, 2, };

static const char hitBlockTable[] = {-1, -2, -1, -1, -1, -1, 0, 0, 1, 1, 1, 1, 2, 2};
constexpr int HIT_BLOCK_TABLE_SIZE = sizeof(hitBlockTable) / sizeof(char);

BlockCoin::BlockCoin(const glm::vec2& _position, uint8_t _ttl)
	: m_count(0)
	, m_ttl(_ttl)
	, m_updated(false)
	, m_animationController()
	, Rect(vec2(BLOCK_COIN_WIDTH, BLOCK_COIN_HEIGHT), _position)
{
	m_animationController.setAnimation(ANIMATION_BLOCK_COIN);
}

BlockCoin::~BlockCoin()
{
}

void BlockCoin::update()
{
	if (m_updated)
		return;

	m_animationController.update();
	
	if (m_count++ <= m_ttl) {
		m_position.y += coinEffectTable[m_count];
	} else {
		m_updated = true;
	}
}

void BlockCoin::draw()
{
	if (m_updated)
		return;

	int texture = g_animations[m_animationController.m_animation].m_keys[m_animationController.m_time];
	g_textureManager.setTexture((TEXTURE)texture);
	Rect::draw();
	g_textureManager.unbindTexture();
}



BlockCoinManager::~BlockCoinManager()
{
}

BlockCoinManager* BlockCoinManager::instance()
{
	static BlockCoinManager instance;
	return &instance;
}

void BlockCoinManager::add(const BlockCoin& _blockCoin)
{
	m_blockCoins.push_back(_blockCoin);
}

void BlockCoinManager::update()
{

	vector<BlockCoin>::iterator iter = m_blockCoins.begin();
	for (; iter != m_blockCoins.end(); iter++) {
		iter->update();
		
		//if (iter->isUpdated()) {
		//	m_blockCoins.erase(iter);
		//} else {
		//	++iter;
		//}
	}


}

void BlockCoinManager::draw()
{
	vector<BlockCoin>::iterator iter = m_blockCoins.begin();
	for (; iter != m_blockCoins.end(); iter++) {
		iter->draw();
	}
}

/** �R���X�g���N�^
*/
HitBlockController::HitBlockController()
	: m_count(0)
	, m_part(PART_NONE)
	, m_endPart(PART_NONE)
	, m_position(0, 0)
	, m_point(0, 0)
	, m_updated(true)
{
}

/** �f�X�g���N�^
*/
HitBlockController::~HitBlockController()
{
}

/** �q�b�g�u���b�N�X�V���X�^�[�g
*	@param	�R�[�X�̈ʒu
*	@param	�X�V����p�[�c
*	@param	�X�V��̃p�[�c
*/
void HitBlockController::start(const glm::ivec2& _point, const PART& _part, const PART& _endPart)
{
	m_point = _point;
	m_position = _point * PART_SIZE;
	m_part = _part;
	m_endPart = _endPart;
	m_count = 0;
	m_updated = false;

	// �n�e�i�u���b�N�̏ꍇ�R�C���̃G�t�F�N�g��`�悷��
	if (_part == PART_QUESTION3) {
		vec2 position = { m_position.x + 4, m_position.y - PART_SIZE - 4 };
		BlockCoinManager* blockCoinMgr = BlockCoinManager::instance();
		BlockCoin blockCoin(position, BLOCK_COIN_TTL);
		blockCoinMgr->add(blockCoin);
	}
}

/** �p�[�c���X�V
*	@param	�X�V���ꂽ��`
*	@return	�X�V�t���O
*/
bool HitBlockController::update(QUAD& _quad)
{
	if (m_updated)
		return false;

	// �J�E���g���Ō�̏ꍇ
	if (m_count >= HIT_BLOCK_TABLE_SIZE) {
		// �p�[�c���Z�b�g����
		g_courseManager.setParts(m_point, m_endPart);

		// �X�V�I���t���O�𗧂Ă�
		m_updated = true;
	}

	// �u���b�N�̈ʒu�����炷
	if (m_count < HIT_BLOCK_TABLE_SIZE) {
		m_position.y += hitBlockTable[m_count];
		m_count++;
	}

	// QUAD��ݒ�
	float leftTopX = (float)g_parts[m_part].m_uvX / 256;
	float leftTopY = (float)g_parts[m_part].m_sizeX / 256;
	float rightBottomX = (float)g_parts[m_part].m_uvY / 256;
	float rightBottomY = (float)g_parts[m_part].m_sizeY / 256;

	_quad.vertices[0].position = { m_position.x, m_position.y };
	_quad.vertices[0].texCoord = { leftTopX, rightBottomX };
	_quad.vertices[1].position = { m_position.x + PART_SIZE, m_position.y };
	_quad.vertices[1].texCoord = { leftTopY, rightBottomX };
	_quad.vertices[2].position = { m_position.x + PART_SIZE, m_position.y + PART_SIZE };
	_quad.vertices[2].texCoord = { leftTopY, rightBottomY };
	_quad.vertices[3].position = { m_position.x, m_position.y + PART_SIZE };
	_quad.vertices[3].texCoord = { leftTopX, rightBottomY };

	return true;
}
