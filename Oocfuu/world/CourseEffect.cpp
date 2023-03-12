#include "CourseEffect.h"
#include "TextureManager.h"
#include "animation/Animation.h"

using namespace glm;
using namespace std;

static const char coinEffectTable[] = { -2,-3,-4, -4, -4, -4, -4, -4, -3, -3, -2, -2, -1, 0, 0,
										0, 0, 1, 2, 2, 3, 3, 4, 4, 4, 4, 4, 4, 3, 2, };

EffectCoin::EffectCoin(const glm::vec2& _position)
	: m_animationController()
	, EffectPartBase(vec2(EFFECT_COIN_WIDTH, EFFECT_COIN_HEIGHT), _position)
{
	m_animationController.setAnimation(ANIMATION_BLOCK_COIN);
	m_ttl = EFFECT_COIN_TTL;
}

EffectCoin::~EffectCoin()
{
}

void EffectCoin::update()
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

void EffectCoin::draw()
{
	if (m_updated)
		return;

	int texture = g_animations[m_animationController.m_animation].m_keys[m_animationController.m_time];
	g_textureManager.setTexture((TEXTURE)texture);
	Rect::draw();
	g_textureManager.unbindTexture();
}

/* スコアエフェクトコンストラクタ
*/
EffectScore::EffectScore(const glm::vec2& _position, SCORE _score)
	: m_score(_score)
	, EffectPartBase(vec2(EFFECT_SCORE_WIDTH, EFFECT_SCORE_HEIGHT),_position)
{
	m_ttl = EFFECT_SCORE_TTL;
}

/* スコアエフェクトデストラクタ
*/
EffectScore::~EffectScore()
{
}

/* スコアエフェクトを更新
*/
void EffectScore::update()
{
	if (m_updated)
		return;

	if (m_count++ <= m_ttl) {
		m_position.y -= 2.0f;
	} else {
		m_updated = true;
	}
}

/* スコアエフェクトを描画
*/
void EffectScore::draw()
{
	if (m_updated)
		return;

	g_textureManager.setTexture(TEXTURE_SCORE_100);
	Rect::draw();
	g_textureManager.unbindTexture();

}

/* デストラクタ
*/
CourseEffectManager::~CourseEffectManager()
{
}

/* 唯一のインスタンスを取得
*	@return インスタンスのポインタ
*/
CourseEffectManager* CourseEffectManager::instance()
{
	static CourseEffectManager instance;
	return &instance;
}

/* エフェクトを更新
*/
void CourseEffectManager::update()
{

	vector<EffectCoin>::iterator iterCoin = m_coins.begin();
	for (; iterCoin != m_coins.end(); ++iterCoin) {
		iterCoin->update();

		// TODO 更新が終わったものは削除する
		//if (iter->isUpdated()) {
		//	m_blockCoins.erase(iter);
		//} else {
		//	++iter;
		//}
	}

	vector<EffectScore>::iterator iterScore = m_scores.begin();
	for (; iterScore != m_scores.end(); ++iterScore) {
		iterScore->update();
	}
}

/* エフェクトを描画
*/
void CourseEffectManager::draw()
{
	vector<EffectCoin>::iterator iterCoin = m_coins.begin();
	for (; iterCoin != m_coins.end(); iterCoin++) {
		iterCoin->draw();
	}

	vector<EffectScore>::iterator iterScore = m_scores.begin();
	for (; iterScore != m_scores.end(); ++iterScore) {
		iterScore->draw();
	}
}