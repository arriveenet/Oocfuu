#pragma once
#include "Game.h"
#include "Rect.h"
#include "Kinopio.h"
#include "common/common.h"
#include "Course.h"

#include <gl/glut.h>
#include <glm/glm.hpp>
#include <Windows.h>

#include <vector>
#include <string>

/**
* コース管理クラス
*/
class CourseManager {
private:
	float					m_scroll;				//!< スクロール位置
	bool					m_isLoaded;				//!< コース読み込みフラグ
	Course					m_currentCourse;		//!< 現在設定されているコース

public:
	Kinopio					m_kinopio;				//!< キノピオ

	// コンストラクタ
	CourseManager();

	// デストラクタ
	virtual ~CourseManager();

	// コース配列を解放
	void release();

	// コースをクリアする
	void clear();

	// コースを更新
	void update();

	// コースを描画
	void draw();

	// スクロール位置を設定
	void setScroll(const float _scroll) { m_scroll = _scroll; };

	// 現在のスクロールを取得
	float getScroll() const { return m_scroll; };

	// 現在のスクロール位置から加算する
	void addScroll(const float _scroll) { m_scroll += _scroll; };

	// コースの幅までスクロールしたか
	bool isScrollMax();

	// パーツを設定
	void setParts(glm::ivec2 const& _point, int _parts);

	// パーツを取得
	int getParts(int _x, int _y);

	// パーツを取得
	int getParts(glm::vec2 const& _point);

	// コースとの当たり判定
	bool intersect(glm::vec2 const& _point);

	// コースのコインとの当たり判定
	void intersectCoin(Player* _pPlayer);

	// ブロックを叩く
	void hitBlock(glm::vec2 const& _point);

	// コースの幅を取得
	int getWidth() const
	{
		// ボスステージの場合幅を縮める
		if ((g_game.m_world.stage == 4) && (!g_player.m_clear)) {
			return m_currentCourse.m_width - 17;
		}
		return m_currentCourse.m_width;
	};

	// コースの高さを取得
	int getHeight() const { return m_currentCourse.m_height; };

	// 背景色を取得する
	COLORREF getClearColor() const { return m_currentCourse.m_clearColor; };

	// プレイヤーのスタート位置を取得
	glm::ivec2 getStartPosition() const { return m_currentCourse.m_startPosition; };

	// 次のワールドを取得
	WORLD getNextWorld() const { return m_currentCourse.m_nextWorld; };

	// クリア斧と当たっているか
	bool getClearAex(Rect& _rect);

	// ボスステージの橋を破壊する
	void destroyBridge()
	{
		m_currentCourse.m_bridgeController.destroy();
	};

	// ボスステージの橋が壊されたか
	bool isBridgeDestroyed()
	{
		return m_currentCourse.m_bridgeController.isDestroyed();
	};

	// キノピオをとの当たり判定
	bool intersectKinopio(const Rect* _rect);

	// 矩形が画面内にいるか
	bool isInScreen(const Rect& _rect);

	// コースをインポートする
	void import(Course* pCourse);
};

extern CourseManager g_courseManager;