#pragma once
#include "Rect.h"
#include "Part.h"
#include "common/common.h"
#include "animation/AnimationController.h"

#include <glm/glm.hpp>
#include <gl/glut.h>
#include <vector>

#define EFFECT_COIN_WIDTH		8		// コインエフェクトの幅
#define EFFECT_COIN_HEIGHT		16		// コインエフェクトの高さ
#define EFFECT_COIN_TTL			24		// コインエフェクトの生存期間

#define EFFECT_SCORE_WIDTH		16
#define EFFECT_SCORE_HEIGHT		8
#define EFFECT_SCORE_TTL		24

/* ゴールフラグクラス
*/
class GoalFlag : public Rect {
	bool m_down;		// 更新フラグ
	bool m_visible;		// 描画フラグ

public:
	GoalFlag();
	~GoalFlag();

	void start() { m_down = true; };
	void stop() { m_down = false; };
	void setVisible(bool _flag) { m_visible = _flag; };
	void update();
	void draw() override;
};

/* エフェクトパーツの抽象クラス
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
	uint8_t m_count = 0;	// カウンター
	uint8_t m_ttl = 0;		// 生存期間
	bool m_updated = false;	// 更新終了フラグ

};

/* コインエフェクトクラス
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

/* スコアエフェクトクラス
*/
class EffectScore : public EffectPartBase {
public:
	// スコアの種類を列挙
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

	// コンストラクタ
	EffectScore(const glm::vec2& _position, SCORE _score);
	// デストラクタ
	~EffectScore();

	// スコアエフェクトを更新
	void update() override;

	// スコアエフェクトを描画
	void draw() override;

private:
	SCORE m_score;
	GLuint m_texture;
};


/* コースエフェクト管理クラス
*/
class CourseEffectManager {
private:
	// コンストラクタ
	CourseEffectManager() {};
	// コピーコンストラクタ
	CourseEffectManager(const CourseEffectManager&) {};

	std::vector<EffectCoin> m_coins;
	std::vector<EffectScore> m_scores;
	GoalFlag m_goalFlag;

public:
	// デストラクタ
	virtual ~CourseEffectManager();

	// 唯一のインスタンスを取得
	static CourseEffectManager* instance();

	// エフェクトをクリアする
	void clear();

	// コインエフェクトを追加
	void addCoin(const EffectCoin& _coin);

	// スコアエフェクトを追加
	void addScore(const EffectScore& _score);

	// ゴールフラグの位置を設定
	void setGoalFlag(const glm::vec2& _position);

	// ゴールフラグの更新をスタート
	void startGoalFlag() { m_goalFlag.start(); };

	// ゴールフラグの更新をストップ
	void stopGoalFlag() { m_goalFlag.start(); };

	// エフェクトを更新
	void update();

	// エフェクトを描画
	void draw();
};