#include "HitBlockController.h"
#include "Course.h"
#include "CourseEffect.h"
#include "sound/Sound.h"

using namespace glm;

// �@���ꂽ�u���b�N�̃e�[�u��
static const char hitBlockTable[] = { -1, -2, -1, -1, -1, -1, 0, 0, 1, 1, 1, 1, 2, 2 };
constexpr int HIT_BLOCK_TABLE_SIZE = sizeof(hitBlockTable) / sizeof(char);

/** �R���X�g���N�^
*/
HitBlockController::HitBlockController()
	: m_count(0)
	, m_part(PART_NONE)
	, m_endPart(PART_NONE)
	, m_position(0, 0)
	, m_point(0, 0)
	, m_updated(true)
	, m_enableEffect(false)
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
	m_enableEffect = false;

	// �@������̃p�[�c���擾����
	int upPart = g_courseManager.getParts(_point.x, _point.y - 1);

	// ��̃p�[�c���R�C�����m�F����
	if (upPart == PART_COIN_0) {
		// �G�t�F�N�g��L���ɂ���
		m_enableEffect = true;
		// �R�C����ǉ�����
		g_game.addCoin();
		g_courseManager.setParts(ivec2(_point.x, _point.y - 1), PART_NONE);
	}

	// �n�e�i�u���b�N�������͏�̃p�[�c���R�C���ꍇ�A�R�C���̃G�t�F�N�g��`�悷��
	if ((m_part == PART_QUESTION3) || m_enableEffect) {
		m_enableEffect = true;
		vec2 position = { m_position.x + 4, m_position.y - PART_SIZE - 4 };
		CourseEffectManager* blockCoinMgr = CourseEffectManager::instance();
		EffectCoin blockCoin(position);
		blockCoinMgr->addCoin(blockCoin);
	} else {
		g_pSound->play(SOUND_SE_BUMP);
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

		// �X�R�A�G�t�F�N�g��ǉ�
		if (m_enableEffect) {
			vec2 scorePosition = { m_position.x, m_position.y - PART_SIZE - 8 };
			EffectScore score(scorePosition, EffectScore::SCORE_200);
			CourseEffectManager* courseEffectMgr = CourseEffectManager::instance();
			courseEffectMgr->addScore(score);
		}
	}

	// �u���b�N�̈ʒu�����炷
	if (m_count < HIT_BLOCK_TABLE_SIZE) {
		m_position.y += hitBlockTable[m_count];
		m_count++;
	}

	// QUAD��ݒ�
	vec2* texCoords = g_partManager.getTexCoords(m_part);

	_quad.vertices[0].position = { m_position.x, m_position.y };
	_quad.vertices[0].texCoord = { texCoords[0].x,  texCoords[0].y };
	_quad.vertices[1].position = { m_position.x, m_position.y + PART_SIZE };
	_quad.vertices[1].texCoord = { texCoords[1].x,  texCoords[1].y };
	_quad.vertices[2].position = { m_position.x + PART_SIZE, m_position.y + PART_SIZE };
	_quad.vertices[2].texCoord = { texCoords[2].x,  texCoords[2].y };
	_quad.vertices[3].position = { m_position.x + PART_SIZE, m_position.y };
	_quad.vertices[3].texCoord = { texCoords[3].x,  texCoords[3].y };

	return true;
}
