#include "CourseEffect.h"
#include "TextureManager.h"
#include "animation/Animation.h"
#include "player/Player.h"
#include "player/PlayerStateGoal.h"

#define GOAL_FLAG_DOWN_HEIGHT	170	// �S�[���t���O���~�낷����

using namespace glm;
using namespace std;

// �R�C���G�t�F�N�g�̈ړ��e�[�u��
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

/* �X�R�A�G�t�F�N�g�R���X�g���N�^
*/
EffectScore::EffectScore(const glm::vec2& _position, SCORE _score)
	: m_score(_score)
	, m_texture(TEXTURE_SCORE_100 + (int)_score)
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

	g_textureManager.setTexture(m_texture);
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

/* �R�[�X�G�t�F�N�g���N���A
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

/* �G�t�F�N�g���X�V
*/
void CourseEffectManager::update()
{
	// �R�C���G�t�F�N�g���X�V
	vector<EffectCoin>::iterator iterCoin = m_coins.begin();
	for (; iterCoin != m_coins.end();) {
		// �X�V�I�����m�F����
		if (iterCoin->isUpdated()) {
			iterCoin = m_coins.erase(iterCoin);
		} else {
			iterCoin->update();
			++iterCoin;
		}
	}

	// �X�R�A�G�t�F�N�g���X�V
	vector<EffectScore>::iterator iterScore = m_scores.begin();
	for (; iterScore != m_scores.end();) {
		// �X�V�I�����m�F����
		if (iterScore->isUpdated()) {
			iterScore = m_scores.erase(iterScore);
		} else {
			iterScore->update();
			++iterScore;
		}
	}

	m_goalFlag.update();
}

/* �G�t�F�N�g��`��
*/
void CourseEffectManager::draw()
{
	// �R�C���G�t�F�N�g��`��
	vector<EffectCoin>::iterator iterCoin = m_coins.begin();
	for (; iterCoin != m_coins.end(); iterCoin++) {
		iterCoin->draw();
	}

	// �X�R�A�G�t�F�N�g��`��
	vector<EffectScore>::iterator iterScore = m_scores.begin();
	for (; iterScore != m_scores.end(); ++iterScore) {
		iterScore->draw();
	}

	m_goalFlag.draw();
}