#include "CourseManager.h"

#include "App.h"
#include "Game.h"
#include "Part.h"
#include "CourseEffect.h"
#include "common/font.h"
#include "player/Player.h"
#include "TextureManager.h"
#include "world/GimmickPart.h"
#include "enemy/EnemyManager.h"
#include "sound/Sound.h"

#include <stdio.h>
#include <iostream>

#define COURSE_ERROR_MSG_NO_ERRO			"No error"
#define COURSE_ERROR_MSG_INVALID_SIZE		"A value less than 0 was specified for the width or height of the course\n"
#define COURSE_ERROR_MSG_INVALID_PARTS		"An invalid part was loaded\n"
#define COURSE_ERROR_MSG_FAILED_OPEN_FILE	"File could not be opened\n"
#define COURSE_ERROR_MSG_OUT_OF_MEMORY		"Failed to allocate memory\n"

using namespace glm;
using namespace std;

CourseManager g_courseManager;

CourseManager::CourseManager()
	: m_scroll(0.0f)
	, m_isLoaded(false)
	, m_hitBlockController()
	, m_clearAex(PART_SIZE, PART_SIZE)
	, m_bridgeController()
	, m_kinopio()
	, m_courseError(COURSE_NO_ERROR)
	, m_errorMsg(COURSE_ERROR_MSG_NO_ERRO)
{
}

CourseManager::~CourseManager()
{
	release();
	clear();
}

void CourseManager::release()
{
	//if (m_pParts && m_isLoaded) {
	//	for (int i = 0; i < m_height; ++i) {
	//		delete m_pParts[i];
	//	}
	//	delete[] m_pParts;
	//	m_isLoaded = false;
	//}

	//m_pParts = nullptr;
}

void CourseManager::clear()
{
	// コースエフェクトマネージャーをクリアする
	CourseEffectManager* courseEffectMgr = CourseEffectManager::instance();
	courseEffectMgr->clear();

	// ボスステージの橋をクリアする
	m_bridgeController.clear();

	// キノピオを無効する
	m_kinopio.m_enable = false;

	// 仕掛けパーツをクリアする
	g_gmmickPart.clear();

	// 敵キャラクターをクリアする
	g_enemyManager.clear();
}

bool CourseManager::load(const char* _fileName)
{
	//FILE* pFile;
	//errno_t error;
	//int width = 0, height = 0, color, startX, startY, nextWorld, nextStage;

	//error = fopen_s(&pFile, _fileName, "r");
	//if (error != 0) {
	//	m_courseError = COURSE_FAILED_OPEN_FILE;
	//	m_errorMsg = COURSE_ERROR_MSG_FAILED_OPEN_FILE;
	//	m_errorMsg += _fileName;
	//	return false;
	//}

	//// コースファイルのヘッダを読み込む
	//fscanf_s(pFile, "width=%d height=%d color=%d startX=%d startY=%d nextWorld=%d nextStage=%d",
	//	&width, &height, &color, &startX, &startY, &nextWorld, &nextStage);

	////printf("/---------------COURSE INFO----------------/\n");
	////printf("width height : %d, %d\n", width, height);
	////printf("clear color : %d, %d, %d\n", GetRValue(color), GetGValue(color), GetBValue(color));
	////printf("start position : %d, %d\n", startX, startY);
	////printf("next world : %d-%d\n", nextWorld, nextStage);
	////printf("/-----------------------------------------/\n");

	//if (width <= 0 || height <= 0) {
	//	m_courseError = COURSE_INVALID_SIZE;
	//	m_errorMsg = COURSE_ERROR_MSG_INVALID_SIZE;
	//	m_isLoaded = false;
	//	return false;
	//}

	//// すでにコースが読み込まれていた場合メモリを解放する
	//if (m_isLoaded && m_pParts) {
	//	// メモリを解放する
	//	release();

	//	// コースをクリアする
	//	clear();
	//}

	//// コースエフェクトマネージャー
	//CourseEffectManager* courseEffectMgr = CourseEffectManager::instance();

	//// メンバ変数に代入する
	//m_width = width;
	//m_height = height;
	//m_clearColor = color;
	//m_startPosition = ivec2(startX, startY);
	//m_nextWorld = { (unsigned char)nextWorld, (unsigned char)nextStage };

	//// メモリを確保
	//try {
	//	m_pParts = new int* [m_height];
	//	for (int i = 0; i < m_height; i++) {
	//		m_pParts[i] = new int[m_width];
	//	}
	//} catch (bad_alloc& ex) {
	//	m_courseError = COURSE_OUT_OF_MEMORY;
	//	m_errorMsg = COURSE_ERROR_MSG_OUT_OF_MEMORY;
	//	m_errorMsg = ex.what();
	//	m_isLoaded = false;
	//	return false;
	//}

	///*
	//* fscanf_sで読み込むとなぜかファイルポインタの位置がずれるので位置を調整する
	//*/
	//rewind(pFile);	// ファイルポインタを最初に戻す
	//char str[256];
	//fgets(str, sizeof str, pFile);	// 一行ファイルポインタを読み込む

	//for (int i = 0; i < m_height; i++) {
	//	for (int j = 0; j < m_width; j++) {
	//		char buf[2];
	//		fread(buf, sizeof(char), 2, pFile);
	//		//printf("[%d-%d] %c%c\n", i, j, buf[0], buf[1]);
	//		if (buf[0] == 0x20) {
	//			m_pParts[i][j] = PART_NONE;
	//		} else {
	//			bool validPart = false;	// 有効なパーツフラグ
	//			for (int k = PART_NONE + 1; k < PART_MAX; k++) {
	//				if (strncmp(buf, g_parts[k].m_fileName, 2) == 0) {
	//					validPart = true;
	//					m_pParts[i][j] = k;

	//					switch (k) {
	//					case PART_AXE_0:
	//						m_clearAex.m_position = { j * PART_SIZE, i * PART_SIZE };
	//						break;
	//					case PART_BRIDGE:
	//						m_bridgeController.add(j, i);
	//						break;
	//					case PART_CHAIN:
	//						m_bridgeController.setChain(j, i);
	//						break;
	//					case PART_GOAL_TOP:
	//						courseEffectMgr->setGoalFlag(vec2(j * PART_SIZE - 8, i * PART_SIZE + 17));
	//						break;
	//					default:
	//						break;
	//					}

	//					break;
	//				}
	//			}
	//			// 有効なパーツ判定
	//			if (!validPart) {
	//				m_courseError = COURSE_INVALID_VALUE;
	//				m_errorMsg = COURSE_ERROR_MSG_INVALID_PARTS;
	//				release();
	//				return false;
	//			}
	//		}
	//	}
	//	fseek(pFile, 2, SEEK_CUR);
	//}


	//// ファイヤーバーの読み込み
	//int firebarCount = 0;
	//if (fscanf_s(pFile, "firebar=%d\n", &firebarCount) != EOF) {
	//	//printf("firebaCount=%d\n", firebarCount);
	//	for (int i = 0; i < firebarCount; i++) {
	//		int x, y, rotate;
	//		fscanf_s(pFile, "x=%d y=%d rotate=%d\n", &x, &y, &rotate);
	//		//printf("Firebar: x=%d, y=%d, rotate=%d\n", x, y, rotate);
	//		g_gmmickPart.addFirebar(Firebar((float)x, (float)y, (FIREBAR_ROTATE)rotate));
	//	}
	//}

	//// リフトの読み込み
	//int liftCount = 0;
	//if (fscanf_s(pFile, "lift=%d\n", &liftCount) != EOF) {
	//	//printf("liftCount=%d\n", liftCount);
	//	for (int i = 0; i < liftCount; i++) {
	//		int x, y, lifWidth, mode;
	//		fscanf_s(pFile, "x=%d y=%d width=%d mode=%d\n", &x, &y, &lifWidth, &mode);
	//		//printf("lift: x=%d, y=%d, width=%d, mode=%d\n", x, y, width, mode);
	//		g_gmmickPart.addLift(Lift((float)x, (float)y, lifWidth, (LIFT_MOVEMENT)mode));
	//	}
	//}

	//// 敵キャラクターの読み込み
	//int enemyCount = 0;
	//int enemyFlag = 0;
	//if (fscanf_s(pFile, "enemyCount=%d flag=%d\n", &enemyCount, &enemyFlag) != EOF) {
	//	//printf("enemyCount=%d, enemyFlag=%d\n", enemyCount, enemyFlag);
	//	g_enemyManager.m_enemyFlag = enemyFlag;
	//	for (int i = 0; i < enemyCount; i++) {
	//		int type, x, y;
	//		fscanf_s(pFile, "type=%d x=%d y=%d\n", &type, &x, &y);
	//		//printf("enemy: type=%d, x=%d, y=%d\n", type, x, y);
	//		ENEMYINFO enemy;
	//		enemy.type = (ENEMYTYPE)type;
	//		enemy.position = ivec2(x, y);
	//		g_enemyManager.addEnemy(enemy);

	//		// クッパであれば範囲を設定する
	//		if (enemy.type == ENEMYTYPE_KOOPA) {
	//			g_enemyManager.setKoopaRange(m_bridgeController.getRange());
	//		}
	//	}
	//}

	//// キノピオの読み込み
	//{
	//	int x = 0; int y = 0;
	//	if (fscanf_s(pFile, "kinopio x=%d y= %d", &x, &y) != EOF) {
	//		m_kinopio.m_enable = true;
	//		m_kinopio.m_position = { x, y };
	//	}
	//}

	//fclose(pFile);

	//m_isLoaded = true;
	//update();

	//m_courseError = COURSE_NO_ERROR;
	return true;
}

void CourseManager::update()
{
	if (!m_isLoaded)
		return;

	m_quads.clear();
	m_coins.clear();

	// ボスステージの橋を更新
	m_bridgeController.update();

	// 叩かれたブロックを更新
	QUAD hitPart;
	if (m_hitBlockController.update(hitPart)) {
		m_quads.push_back(hitPart);
	}

	// ブロックを叩いたときのコインを更新
	CourseEffectManager* courseEffectMgr = CourseEffectManager::instance();
	courseEffectMgr->update();

	m_currentCourse.update();
}

void CourseManager::draw()
{
	m_currentCourse.draw();

	// ブロックを叩いたときのコインを描画
	CourseEffectManager* courseEffectMgr = CourseEffectManager::instance();
	courseEffectMgr->draw();

	// キノピオを描画
	m_kinopio.draw();
}

bool CourseManager::isScrollMax()
{
	if (m_scroll >= ((m_currentCourse.getWidth() * PART_SIZE) - SCREEN_WIDTH)) {
		return true;
	}

	return false;
}

void CourseManager::setParts(ivec2 const& _point, int _parts)
{
	if (
		(_point.x < 0)
		|| (_point.x >= m_currentCourse.getWidth())
		|| (_point.y < 0)
		|| (_point.y >= m_currentCourse.getHeight())
		)
		return;

	m_currentCourse.m_pParts[_point.y][_point.x] = _parts;
}

int CourseManager::getParts(int _x, int _y)
{
	if (
		(_x < 0)
		|| (_x >= m_currentCourse.getWidth())
		|| (_y < 0)
		|| (_y >= m_currentCourse.getHeight())
		)
		return PART_NONE;

	return m_currentCourse.m_pParts[_y][_x];
}

int CourseManager::getParts(glm::vec2 const& _point)
{
	ivec2 cellPoint = (ivec2)_point / PART_SIZE;
	if (
		(cellPoint.x < 0)
		|| (cellPoint.x >= m_currentCourse.getWidth())
		|| (cellPoint.y < 0)
		|| (cellPoint.y >= m_currentCourse.getHeight())
		)
		return PART_NONE;

	return m_currentCourse.m_pParts[cellPoint.y][cellPoint.x];
}

bool CourseManager::intersect(vec2 const& _point)
{
	ivec2 cellPoint = (ivec2)_point / PART_SIZE;
	if (
		(cellPoint.x < 0)
		|| (cellPoint.x >= m_currentCourse.m_width)
		|| (cellPoint.y < 0)
		|| (cellPoint.y >= m_currentCourse.m_height)
		)
		return false;

	switch (m_currentCourse.m_pParts[cellPoint.y][cellPoint.x]) {
	case PART_GROUND:
	case PART_HARD_BLOCK:
	case PART_SOFT_BLOCK:
	case PART_UNDER_GROUND:
	case PART_UNDER_HARD_BLOCK:
	case PART_UNDER_SOFT_BLOCK:
	case PART_SIDE_PIPE_UP_LEFT:
	case PART_SIDE_PIPE_DOWN_LEFT:
	case PART_SIDE_PIPE_UP:
	case PART_SIDE_PIPE_DOWN:
	case PART_SIDE_PIPE_JOINT_UP:
	case PART_SIDE_PIPE_JOINT_DOWN:
	case PART_PIPE_UP_LEFT:
	case PART_PIPE_UP_RIGHT:
	case PART_PIPE_DOWN_LEFT:
	case PART_PIPE_DOWN_RIGHT:
	case PART_QUESTION0:
	case PART_QUESTION1:
	case PART_QUESTION2:
	case PART_QUESTION3:
	case PART_CASTLE_GROUND:
	case PART_WOOD_0:
	case PART_WOOD_1:
	case PART_WOOD_2:
	case PART_BRIDGE:
		return true;
	}
	return false;
}

void CourseManager::intersectCoin(Player* _pPlayer)
{
	//int scrolleColumn = (int)m_scroll / PART_SIZE;
	vector<ivec2>::iterator iter = m_coins.begin();
	for (; iter != m_coins.end(); ++iter) {
		Rect coin(vec2(PART_SIZE, PART_SIZE), vec2(iter->x * PART_SIZE, iter->y * PART_SIZE));
		if (_pPlayer->intersect(coin)) {
			g_game.addCoin();
			setParts(ivec2(iter->x, iter->y), PART_NONE);
		}
	}
}

void CourseManager::hitBlock(glm::vec2 const& _point)
{
	int part, endPart;

	ivec2 cellPoint = (ivec2)_point / PART_SIZE;
	int cellPart = getParts(_point);

	switch (cellPart) {
	case PART_QUESTION0:
		part = endPart = PART_QUESTION3;
		g_game.addCoin();
		break;
	case PART_SOFT_BLOCK:
		part = endPart = PART_SOFT_BLOCK;
		break;
	case PART_UNDER_SOFT_BLOCK:
		part = endPart = PART_UNDER_SOFT_BLOCK;
		break;
	default:
		g_pSound->play(SOUND_SE_BUMP);
		return;
	}

	setParts(cellPoint, PART_NONE);
	m_hitBlockController.start(cellPoint, (PART)part, (PART)endPart);

	g_enemyManager.hitBlock(cellPoint);
}

bool CourseManager::getClearAex(Rect& _rect)
{
	if (m_clearAex.m_position != vec2(0, 0)) {
		_rect = m_clearAex;
		return true;
	}
	return false;
}

bool CourseManager::intersectKinopio(const Rect* _rect)
{
	if (m_kinopio.m_enable && m_kinopio.intersect(*_rect)) {
		return true;
	}
	return false;
}

bool CourseManager::isInScreen(const Rect& _rect)
{
	if (_rect.m_position.x > m_scroll
		&& _rect.m_position.x < m_scroll + SCREEN_WIDTH) {
		return true;
	}

	return false;
}

void CourseManager::import(Course* pCourse)
{
	m_currentCourse = *pCourse;
}
