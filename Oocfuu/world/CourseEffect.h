#pragma once
#include "Rect.h"
#include "Part.h"
#include "common/common.h"
#include "animation/AnimationController.h"

#include <glm/glm.hpp>
#include <gl/glut.h>
#include <vector>

#define EFFECT_COIN_WIDTH		8		// �R�C���G�t�F�N�g�̕�
#define EFFECT_COIN_HEIGHT		16		// �R�C���G�t�F�N�g�̍���
#define EFFECT_COIN_TTL			24		// �R�C���G�t�F�N�g�̐�������

#define EFFECT_SCORE_WIDTH		16
#define EFFECT_SCORE_HEIGHT		8
#define EFFECT_SCORE_TTL		24

/* �S�[���t���O�N���X
*/
class GoalFlag : public Rect {
	bool m_down;		// �X�V�t���O
	bool m_visible;		// �`��t���O

public:
	GoalFlag();
	~GoalFlag();

	void start() { m_down = true; };
	void stop() { m_down = false; };
	void setVisible(bool _flag) { m_visible = _flag; };
	void update();
	void draw() override;
};

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

/* �R�C���G�t�F�N�g�N���X
*/
class EffectCoin : public EffectPartBase {
public:
	explicit EffectCoin(const glm::vec2& _position);
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
	GLuint m_texture;
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
	GoalFlag m_goalFlag;

public:
	// �f�X�g���N�^
	virtual ~CourseEffectManager();

	// �B��̃C���X�^���X���擾
	static CourseEffectManager* instance();

	// �G�t�F�N�g���N���A����
	void clear();

	// �R�C���G�t�F�N�g��ǉ�
	void addCoin(const EffectCoin& _coin);

	// �X�R�A�G�t�F�N�g��ǉ�
	void addScore(const EffectScore& _score);

	// �S�[���t���O�̈ʒu��ݒ�
	void setGoalFlag(const glm::vec2& _position);

	// �S�[���t���O�̍X�V���X�^�[�g
	void startGoalFlag() { m_goalFlag.start(); };

	// �S�[���t���O�̍X�V���X�g�b�v
	void stopGoalFlag() { m_goalFlag.start(); };

	// �G�t�F�N�g���X�V
	void update();

	// �G�t�F�N�g��`��
	void draw();
};