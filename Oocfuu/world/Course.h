#pragma once
#include "Game.h"
#include "Rect.h"
#include "HitBlockController.h"
#include "BridgeController.h"
#include "Kinopio.h"
#include "common/common.h"

#include <gl/glut.h>
#include <glm/glm.hpp>
#include <Windows.h>

#include <vector>
#include <string>

// コースの読み込み時のエラーを列挙する
enum COURSE_ERROR {
	COURSE_NO_ERROR = 0,		// エラーなし
	COURSE_INVALID_VALUE,		// 無効なパーツが指定されている
	COURSE_INVALID_SIZE,		// コースのサイズが0以下
	COURSE_FAILED_OPEN_FILE,	// コースファイルが開けなかった
	COURSE_OUT_OF_MEMORY,		// コースデータを読み込むメモリが残っていない
};

// コースファイルのヘッダー構造体
typedef struct COURSEINFOHEADER {
	unsigned short width;		// コースの幅
	unsigned short height;		// コースの高さ
	COLORREF clearColor;		// バッファのクリアカラー
	unsigned short startX;		// プレイヤーのスタートX座標
	unsigned short startY;		// プレイヤーのスタートY座標
	BYTE nextWorld;
	BYTE nextStage;
} COURSEINFOHEADER, *LPCOURSEINFOHEADER;

/* コース管理クラス
*/
class CourseManager {
private:
	int m_width;								// コースの幅
	int m_height;								// コースの高さ
	float m_scroll;								// スクロール位置
	COLORREF m_clearColor;						// 背景色
	glm::ivec2 m_startPosition;					// プレイヤーのスタート位置
	WORLD m_nextWorld;							// 次のワールド
	int** m_pParts;								// コースパーツ
	bool m_isLoaded;							// コース読み込みフラグ
	std::vector<QUAD> m_quads;					// コースパーツの矩形
	std::vector<glm::ivec2> m_coins;			// コインの位置
	HitBlockController m_hitBlockController;	// 叩いたときのブロック制御
	Rect m_clearAex;							// クリア斧
	BridgeController m_bridgeController;		// 橋の制御
	Kinopio m_kinopio;						// キノピオ
	COURSE_ERROR m_courseError;					// コースエラー
	std::string m_errorMsg;						// エラーメッセージ

public:
	// コンストラクタ
	CourseManager();

	// デストラクタ
	virtual ~CourseManager();

	// コース配列を解放
	void release();

	// コースを読み込む
	bool load(const char* _fileName);

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
			return m_width - 17;
		}
		return m_width;
	};

	// コースの高さを取得
	int getHeight() const { return m_height; };

	// 背景色を取得する
	COLORREF getClearColor() const { return m_clearColor; };

	// プレイヤーのスタート位置を取得
	glm::ivec2 getStartPosition() const { return m_startPosition; };

	// 次のワールドを取得
	WORLD getNextWorld() const { return m_nextWorld; };

	// クリア斧と当たっているか
	bool getClearAex(Rect& _rect);

	// ボスステージの橋を破壊する
	void destroyBridge()
	{
		m_bridgeController.destroy();
	};

	// ボスステージの橋が壊されたか
	bool isBridgeDestroyed()
	{
		return m_bridgeController.isDestroyed();
	};

	// キノピオを取得
	Kinopio* getKinopio()
	{
		return &m_kinopio;
	}

	// エラー列挙定数を取得
	COURSE_ERROR getError() const
	{
		return m_courseError;
	};

	// エラーメッセージを取得
	std::string getErrorString() const
	{
		//cout << "m_errorMsg=" << m_errorMsg << endl;
		return m_errorMsg;
	};
};

extern CourseManager g_courseManager;