#include "CourseEffect.h"
#include "TextureManager.h"
#include "animation/Animation.h"
#include "player/Player.h"
#include "player/PlayerStateGoal.h"

#define GOAL_FLAG_DOWN_HEIGHT	170	// ゴールフラグを降ろす高さ

using namespace glm;
using namespace std;

// コインエフェクトの移動テーブル
static const char coinEffectTable[] = { -2,-3,-4, -4, -4, -4, -4, -4, -3, -3, -2, -2, -1, 0, 0,
										0, 0, 1, 2, 2, 3, 3, 4, 4, 4, 4, 4, 4, 3, 2, };


GoalFlag::GoalFlag()
	: m_down(false)
	, m_visible(false)
	, Rect(vec2(PART_SIZE, PART_SIZE), vec2(0.0f, 0.0f))
{
}

GoalFlag::~GoalFlag()
{
}

void GoalFlag::update()
{
	if (m_down) {
		if (m_position.y <= GOAL_FLAG_DOWN_HEIGHT) {
			m_position.y += PLAYER_POLE_FALL_SPEED;
		} else {
			m_down = false;
		}
	}

}

void GoalFlag::draw()
{
	if (!m_visible)
		return;

	g_textureManager.setTexture(TEXTURE_GOAL_FLAG);
	Rect::draw();
	g_textureManager.unbindTexture();
}


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
	, m_texture(TEXTURE_SCORE_100 + (int)_score)
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

	g_textureManager.setTexture(m_texture);
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

/* コースエフェクトをクリア
*/
void CourseEffectManager::clear()
{
	m_coins.clear();
	m_scores.clear();
	m_goalFlag.stop();
	m_goalFlag.setVisible(false);
}

void CourseEffectManager::addCoin(const EffectCoin& _coin)
{
	m_coins.push_back(_coin);
}

void CourseEffectManager::addScore(const EffectScore& _score)
{
	m_scores.push_back(_score);
}

void CourseEffectManager::setGoalFlag(const glm::vec2& _position)
{
	m_goalFlag.m_position = _position;
	m_goalFlag.stop();
	m_goalFlag.setVisible(true);
}

/* エフェクトを更新
*/
void CourseEffectManager::update()
{
	// コインエフェクトを更新
	vector<EffectCoin>::iterator iterCoin = m_coins.begin();
	for (; iterCoin != m_coins.end();) {
		// 更新終了か確認する
		if (iterCoin->isUpdated()) {
			iterCoin = m_coins.erase(iterCoin);
		} else {
			iterCoin->update();
			++iterCoin;
		}
	}

	// スコアエフェクトを更新
	vector<EffectScore>::iterator iterScore = m_scores.begin();
	for (; iterScore != m_scores.end();) {
		// 更新終了か確認する
		if (iterScore->isUpdated()) {
			iterScore = m_scores.erase(iterScore);
		} else {
			iterScore->update();
			++iterScore;
		}
	}

	m_goalFlag.update();
}

/* エフェクトを描画
*/
void CourseEffectManager::draw()
{
	// コインエフェクトを描画
	vector<EffectCoin>::iterator iterCoin = m_coins.begin();
	for (; iterCoin != m_coins.end(); iterCoin++) {
		iterCoin->draw();
	}

	// スコアエフェクトを描画
	vector<EffectScore>::iterator iterScore = m_scores.begin();
	for (; iterScore != m_scores.end(); ++iterScore) {
		iterScore->draw();
	}

	m_goalFlag.draw();
}