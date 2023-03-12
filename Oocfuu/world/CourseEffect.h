#pragma once
#include "Rect.h"
#include "Part.h"
#include "common/common.h"
#include "animation/AnimationController.h"

#include <glm/glm.hpp>
#include <vector>

#define EFFECT_COIN_WIDTH		8		// �R�C���G�t�F�N�g�̕�
#define EFFECT_COIN_HEIGHT		16		// �R�C���G�t�F�N�g�̍���
#define EFFECT_COIN_TTL			24		// �R�C���G�t�F�N�g�̐�������

#define EFFECT_SCORE_WIDTH		16
#define EFFECT_SCORE_HEIGHT		8
#define EFFECT_SCORE_TTL		24

/* �G�t�F�N�g�p�[�c�̒��ۃN���X
*/
class EffectPartBase : public Rect {
public:
	EffectPartBase() {};
	EffectPartBase(glm::vec2 _size, glm::vec2 _position)
	: Rect(_size, _position) {};
	virtual ~EffectPartBase() {};

	virtual void update() = 0;
	virtual bool isUpdated() { return m_updated; };

protected:
	uint8_t m_count = 0;	// �J�E���^�[
	uint8_t m_ttl = 0;		// ��������
	bool m_updated = false;	// �X�V�I���t���O

};

class EffectCoin : public EffectPartBase {
public:
	EffectCoin(const glm::vec2& _position);
	virtual ~EffectCoin();

	void update() override;
	void draw() override;

protected:
	AnimationController m_animationController;

};

/* �X�R�A�G�t�F�N�g�N���X
*/
class EffectScore : public EffectPartBase {
public:
	// �X�R�A�̎�ނ��
	enum SCORE {
		SCORE_100,
		SCORE_200,
		SCORE_400,
		SCORE_500,
		SCORE_800,
		SCORE_1000,
		SCORE_2000,
		SCORE_4000,
		SCORE_5000,
		SCORE_8000,
		SCORE_MAX
	};

	// �R���X�g���N�^
	EffectScore(const glm::vec2& _position, SCORE _score);
	// �f�X�g���N�^
	~EffectScore();

	// �X�R�A�G�t�F�N�g���X�V
	void update() override;

	// �X�R�A�G�t�F�N�g��`��
	void draw() override;

private:
	SCORE m_score;
};


/* �R�[�X�G�t�F�N�g�Ǘ��N���X
*/
class CourseEffectManager {
private:
	// �R���X�g���N�^
	CourseEffectManager() {};
	// �R�s�[�R���X�g���N�^
	CourseEffectManager(const CourseEffectManager&) {};

	std::vector<EffectCoin> m_coins;
	std::vector<EffectScore> m_scores;
public:
	// �f�X�g���N�^
	virtual ~CourseEffectManager();

	// �B��̃C���X�^���X���擾
	static CourseEffectManager* instance();

	// �G�t�F�N�g��ǉ�
	void addCoin(const EffectCoin& _coin)
	{
		m_coins.push_back(_coin);
	}

	void addScore(const EffectScore& _score)
	{
		m_scores.push_back(_score);
	}

	// �G�t�F�N�g���X�V
	void update();

	// �G�t�F�N�g��`��
	void draw();
};