#pragma once
#include "Game.h"
#include "Rect.h"
#include "HitBlockController.h"
#include "BridgeController.h"
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

typedef struct COURSEINFOHEADER {
	unsigned short width;		// コースの幅
	unsigned short height;		// コースの高さ
	COLORREF clearColor;		// バッファのクリアカラー
	unsigned short startX;		// プレイヤーのスタートX座標
	unsigned short startY;		// プレイヤーのスタートY座標
	BYTE nextWorld;
	BYTE nextStage;
} COURSEINFOHEADER, *LPCOURSEINFOHEADER;


class CourseManager {
private:
	int m_width;
	int m_height;
	COLORREF m_clearColor;
	glm::ivec2 m_startPosition;
	WORLD m_nextWorld;
	int** m_pParts;
	bool m_isLoaded;
	std::vector<QUAD> m_quads;
	std::vector<glm::ivec2> m_coins;
	HitBlockController m_hitBlockController;
	Rect m_clearAex;
	BridgeController m_bridgeController;
	COURSE_ERROR m_courseError;
	std::string m_errorMsg;

public:
	float m_scroll;

	CourseManager();
	virtual ~CourseManager();

	void release();
	bool load(const char* _fileName);
	void update();
	void draw();
	void setParts(glm::ivec2 const& _point, int _parts);
	int getParts(int _x, int _y);
	int getParts(glm::vec2 const& _point);
	bool intersect(glm::vec2 const& _point);
	void intersectCoin(Player* _pPlayer);
	void hitBlock(glm::vec2 const& _point);
	int getWidth();
	int getHeight();
	COLORREF getClearColor();
	glm::ivec2 getStartPosition();
	WORLD getNextWorld();
	bool getClearAex(Rect& _rect);
	void destroyBridge();
	bool isBridgeDestroyed();
	COURSE_ERROR getError() const;
	std::string getErrorString() const;
};

extern CourseManager g_courseManager;