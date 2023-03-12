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

/* �X�R�A�G�t�F�N�g�R���X�g���N�^
*/
EffectScore::EffectScore(const glm::vec2& _position, SCORE _score)
	: m_score(_score)
	, EffectPartBase(vec2(EFFECT_SCORE_WIDTH, EFFECT_SCORE_HEIGHT),_position)
{
	m_ttl = EFFECT_SCORE_TTL;
}

/* �X�R�A�G�t�F�N�g�f�X�g���N�^
*/
EffectScore::~EffectScore()
{
}

/* �X�R�A�G�t�F�N�g���X�V
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

/* �X�R�A�G�t�F�N�g��`��
*/
void EffectScore::draw()
{
	if (m_updated)
		return;

	g_textureManager.setTexture(TEXTURE_SCORE_100);
	Rect::draw();
	g_textureManager.unbindTexture();

}

/* �f�X�g���N�^
*/
CourseEffectManager::~CourseEffectManager()
{
}

/* �B��̃C���X�^���X���擾
*	@return �C���X�^���X�̃|�C���^
*/
CourseEffectManager* CourseEffectManager::instance()
{
	static CourseEffectManager instance;
	return &instance;
}

/* �G�t�F�N�g���X�V
*/
void CourseEffectManager::update()
{

	vector<EffectCoin>::iterator iterCoin = m_coins.begin();
	for (; iterCoin != m_coins.end(); ++iterCoin) {
		iterCoin->update();

		// TODO �X�V���I��������͍̂폜����
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

/* �G�t�F�N�g��`��
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