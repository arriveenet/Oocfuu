#include "HitBlockController.h"
#include "Course.h"
#include "CourseEffect.h"

using namespace glm;

// 叩かれたブロックのテーブル
static const char hitBlockTable[] = { -1, -2, -1, -1, -1, -1, 0, 0, 1, 1, 1, 1, 2, 2 };
constexpr int HIT_BLOCK_TABLE_SIZE = sizeof(hitBlockTable) / sizeof(char);

/** コンストラクタ
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

/** デストラクタ
*/
HitBlockController::~HitBlockController()
{
}

/** ヒットブロック更新をスタート
*	@param	コースの位置
*	@param	更新するパーツ
*	@param	更新後のパーツ
*/
void HitBlockController::start(const glm::ivec2& _point, const PART& _part, const PART& _endPart)
{
	m_point = _point;
	m_position = _point * PART_SIZE;
	m_part = _part;
	m_endPart = _endPart;
	m_count = 0;
	m_updated = false;

	// ハテナブロックの場合コインのエフェクトを描画する
	if (m_part == PART_QUESTION3) {
		vec2 position = { m_position.x + 4, m_position.y - PART_SIZE - 4 };
		CourseEffectManager* blockCoinMgr = CourseEffectManager::instance();
		EffectCoin blockCoin(position);
		blockCoinMgr->addCoin(blockCoin);
	}
}

/** パーツを更新
*	@param	更新された矩形
*	@return	更新フラグ
*/
bool HitBlockController::update(QUAD& _quad)
{
	if (m_updated)
		return false;

	// カウントが最後の場合
	if (m_count >= HIT_BLOCK_TABLE_SIZE) {
		// パーツをセットする
		g_courseManager.setParts(m_point, m_endPart);

		// 更新終了フラグを立てる
		m_updated = true;

		// スコアエフェクトを追加
		if (m_part == PART_QUESTION3) {
			vec2 scorePosition = { m_position.x, m_position.y - PART_SIZE - 8 };
			EffectScore score(scorePosition, EffectScore::SCORE_200);
			CourseEffectManager* courseEffectMgr = CourseEffectManager::instance();
			courseEffectMgr->addScore(score);
		}
	}

	// ブロックの位置をずらす
	if (m_count < HIT_BLOCK_TABLE_SIZE) {
		m_position.y += hitBlockTable[m_count];
		m_count++;
	}

	// QUADを設定
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
