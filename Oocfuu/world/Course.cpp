#include "Course.h"

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
#define COURSE_ERROR_MSG_FAILED_OPEN_FILE	"File could not be opened\n"
#define COURSE_ERROR_MSG_OUT_OF_MEMORY		"Failed to allocate memory\n"

using namespace glm;
using namespace std;

CourseManager g_courseManager;

CourseManager::CourseManager()
	: m_scroll(0.0f)
	, m_width(0)
	, m_height(0)
	, m_clearColor(0)
	, m_startPosition(0, 0)
	, m_nextWorld{1, 1}
	, m_pParts(nullptr)
	, m_isLoaded(false)
	, m_hitBlockController()
	, m_clearAex(PART_SIZE, PART_SIZE)
	, m_bridgeController()
	, m_courseError(COURSE_NO_ERROR)
	, m_errorMsg(COURSE_ERROR_MSG_NO_ERRO)
{
}

CourseManager::~CourseManager()
{

}

void CourseManager::release()
{
	if (m_pParts && m_isLoaded) {
		for (int i = 0; i < m_height; ++i) {
			delete m_pParts[i];
		}
		delete[] m_pParts;
		m_isLoaded = false;
	}

	m_pParts = NULL;
	m_quads.~vector();
}

bool CourseManager::load(const char* _fileName)
{
	FILE* pFile;
	errno_t error;
	int width = 0, height = 0, color, startX, startY, nextWorld, nextStage;

	error = fopen_s(&pFile, _fileName, "r");
	if (error != 0) {
		m_courseError = COURSE_FAILED_OPEN_FILE;
		m_errorMsg = COURSE_ERROR_MSG_FAILED_OPEN_FILE;
		m_errorMsg += _fileName;
		return false;
	}

	// コースファイルのヘッダを読み込む
	fscanf_s(pFile, "width=%d height=%d color=%d startX=%d startY=%d nextWorld=%d nextStage=%d",
		&width, &height, &color, &startX, &startY, &nextWorld, &nextStage);

	//printf("/---------------COURSE INFO----------------/\n");
	//printf("width height : %d, %d\n", width, height);
	//printf("clear color : %d, %d, %d\n", GetRValue(color), GetGValue(color), GetBValue(color));
	//printf("start position : %d, %d\n", startX, startY);
	//printf("next world : %d-%d\n", nextWorld, nextStage);
	//printf("/-----------------------------------------/\n");

	if (width <= 0 || height <= 0) {
		m_courseError = COURSE_INVALID_SIZE;
		m_errorMsg = COURSE_ERROR_MSG_INVALID_SIZE;
		m_isLoaded = false;
		return false;
	}

	// すでにコースが読み込まれていた場合メモリを開放する
	if (m_isLoaded && m_pParts) {
		for (int i = 0; i < m_height; ++i) {
			delete m_pParts[i];
		}
		delete[] m_pParts;
		m_pParts = NULL;
	}

	// コースエフェクトマネージャーをクリアする
	CourseEffectManager* courseEffectMgr = CourseEffectManager::instance();
	courseEffectMgr->clear();

	// ボスステージの橋をクリアする
	m_bridgeController.clear();

	// メンバ変数に代入する
	m_width = width;
	m_height = height;
	m_clearColor = color;
	m_startPosition = ivec2(startX, startY);
	m_nextWorld = { (unsigned char)nextWorld, (unsigned char)nextStage };

	// メモリを確保
	try {
		m_pParts = new int* [m_height];
		for (int i = 0; i < m_height; i++) {
			m_pParts[i] = new int[m_width];
		}
	} catch (bad_alloc& ex) {
		m_courseError = COURSE_OUT_OF_MEMORY;
		m_errorMsg = COURSE_ERROR_MSG_OUT_OF_MEMORY;
		m_errorMsg = ex.what();
		m_isLoaded = false;
		return false;
	}

	/*
	* fscanf_sで読み込むとなぜかファイルポインタの位置がずれるので位置を調整する
	*/
	rewind(pFile);	// ファイルポインタを最初に戻す
	char str[256];
	fgets(str, sizeof str, pFile);	// 一行ファイルポインタを読み込む

	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			char buf[2];
			fread(buf, sizeof(char), 2, pFile);
			//printf("[%d-%d] %c%c\n", i, j, buf[0], buf[1]);
			if (buf[0] == 0x20) {
				m_pParts[i][j] = PART_NONE;
			} else
				for (int k = PART_NONE + 1; k < PART_MAX; k++) {
					if (strncmp(buf, g_parts[k].m_fileName, 2) == 0) {
						m_pParts[i][j] = k;

						switch (k) {
						case PART_AXE_0:
							m_clearAex.m_position = { j * PART_SIZE, i * PART_SIZE };
							break;
						case PART_BRIDGE:
							m_bridgeController.add(j, i);
							break;
						case PART_CHAIN:
							m_bridgeController.setChain(j, i);
							break;
						case PART_GOAL_TOP:
							courseEffectMgr->setGoalFlag(vec2(j * PART_SIZE - 8, i * PART_SIZE + 17));
							break;
						default:
							break;
						}

						break;
					}
				}
		}
		fseek(pFile, 2, SEEK_CUR);
	}

	// 仕掛けパーツをクリアする
	g_gmmickPart.clear();

	// ファイヤーバーの読み込み
	int firebarCount = 0;
	if (fscanf_s(pFile, "firebar=%d\n", &firebarCount) != EOF) {
		//printf("firebaCount=%d\n", firebarCount);
		for (int i = 0; i < firebarCount; i++) {
			int x, y, rotate;
			fscanf_s(pFile, "x=%d y=%d rotate=%d\n", &x, &y, &rotate);
			//printf("Firebar: x=%d, y=%d, rotate=%d\n", x, y, rotate);
			g_gmmickPart.addFirebar(Firebar((float)x, (float)y, (FIREBAR_ROTATE)rotate));
		}
	}

	// リフトの読み込み
	int liftCount = 0;
	if (fscanf_s(pFile, "lift=%d\n", &liftCount) != EOF) {
		//printf("liftCount=%d\n", liftCount);
		for (int i = 0; i < liftCount; i++) {
			int x, y, width, mode;
			fscanf_s(pFile, "x=%d y=%d width=%d, mode=%d\n", &x, &y, &width, &mode);
			//printf("lift: x=%d, y=%d, width=%d, mode=%d\n", x, y, width, mode);
			g_gmmickPart.addLift(Lift((float)x, (float)y, width, (LIFT_MOVEMENT)mode));
		}
	}

	// 敵キャラクターの読み込み
	g_enemyManager.reset();
	int enemyCount = 0;
	int enemyFlag = 0;
	if (fscanf_s(pFile, "enemyCount=%d flag=%d\n", &enemyCount, &enemyFlag) != EOF) {
		//printf("enemyCount=%d, enemyFlag=%d\n", enemyCount, enemyFlag);
		g_enemyManager.m_enemyFlag = enemyFlag;
		for (int i = 0; i < enemyCount; i++) {
			int type, x, y;
			fscanf_s(pFile, "type=%d x=%d y=%d\n", &type, &x, &y);
			//printf("enemy: type=%d, x=%d, y=%d\n", type, x, y);
			ENEMYINFO enemy;
			enemy.type = (ENEMYTYPE)type;
			enemy.position = ivec2(x, y);
			g_enemyManager.addEnemy(enemy);

			// クッパであれば範囲を設定する
			if (enemy.type == ENEMYTYPE_KOOPA) {
				g_enemyManager.setKoopaRange(m_bridgeController.getRange());
			}
		}
	}

	fclose(pFile);

	m_isLoaded = true;
	update();

	m_courseError = COURSE_NO_ERROR;
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
	QUAD hitBlock;
	if (m_hitBlockController.update(hitBlock)) {
		m_quads.push_back(hitBlock);
	}

	// ブロックを叩いたときのコインを更新
	CourseEffectManager* courseEffectMgr = CourseEffectManager::instance();
	courseEffectMgr->update();

	int scrolleColumn = (int)m_scroll / PART_SIZE;
	//printf("scrolleColumn=%d\n", scrolleColumn);
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			int part = m_pParts[y][x];
			if (
				(x < scrolleColumn)
				|| (x > scrolleColumn + (SCREEN_WIDTH / PART_SIZE))
				|| (part == PART_NONE)
				)
				continue;

			int textureIndex = part;
			switch (part) {
			case PART_COIN_0:
				m_coins.push_back(ivec2(x, y));
			case PART_QUESTION0:
			case PART_AXE_0:
			{
				int animationTable[] = { 0,1,2,2,1,0 };
				int animationTableLength = sizeof(animationTable) / sizeof(int);
				textureIndex += animationTable[(Game::m_count / 8) % animationTableLength];
			}
				break;
			case PART_SEA_0:
			{
				int animationTable[] = { 0,1,2,3,4,5,6,7 };
				int animationTableLength = sizeof(animationTable) / sizeof(int);
				textureIndex += animationTable[(Game::m_count / 16) % animationTableLength];
			}
				break;
			case PART_DESERT_1:
			{
				int animationTable[] = { 0,1,2,3,4,5,6,7 };
				int animationTableLength = sizeof(animationTable) / sizeof(int);
				textureIndex += animationTable[(Game::m_count / 16) % animationTableLength];

			}
				break;
			}

			float x2 = (float)x * PART_SIZE;
			float y2 = (float)y * PART_SIZE;

			float tx0 = (float)g_parts[textureIndex].m_uvX / 256;
			float tx1 = (float)g_parts[textureIndex].m_sizeX / 256;
			float ty0 = (float)g_parts[textureIndex].m_uvY / 256;
			float ty1 = (float)g_parts[textureIndex].m_sizeY / 256;

			QUAD quad;
			quad.vertices[0].position = { x2, y2 };
			quad.vertices[0].texCoord = { tx0, ty0 };
			quad.vertices[1].position = { x2 + PART_SIZE,y2 };
			quad.vertices[1].texCoord = { tx1, ty0 };
			quad.vertices[2].position = { x2 + PART_SIZE, y2 + PART_SIZE };
			quad.vertices[2].texCoord = { tx1, ty1 };
			quad.vertices[3].position = { x2, y2 + PART_SIZE };
			quad.vertices[3].texCoord = { tx0, ty1 };
			m_quads.push_back(quad);
		}
	}
}

void CourseManager::draw()
{
	if (m_quads.empty() || !m_isLoaded)
		return;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(2, GL_FLOAT, sizeof(VERTEX), &m_quads[0].vertices[0].position);
	glTexCoordPointer(2, GL_FLOAT, sizeof(VERTEX), &m_quads[0].vertices[0].texCoord);

	g_textureManager.setTexture(TEXTURE_PARTS);
	glDrawArrays(GL_QUADS, 0, GLsizei(m_quads.size() * 4));
	g_textureManager.unbindTexture();

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_CULL_FACE);

	// ブロックを叩いたときのコインを描画
	CourseEffectManager* courseEffectMgr = CourseEffectManager::instance();
	courseEffectMgr->draw();

	if (Game::m_debugInfo) {
		fontBegin();
		fontBackgroundColor(true);
		fontColor(0x00, 0xff, 0x00);
		fontPosition(0, 8);
		fontDraw("SCROLL:%f", m_scroll);
		fontColor(0xff, 0xff, 0xff);
		fontBackgroundColor(false);
		fontEnd();
	}
}

void CourseManager::setParts(ivec2 const& _point, int _parts) {
	if (
		(_point.x < 0)
		|| (_point.x >= m_width)
		|| (_point.y < 0)
		|| (_point.y >= m_height)
		)
		return;

	m_pParts[_point.y][_point.x] = _parts;
}

int CourseManager::getParts(int _x, int _y) {
	if (
		(_x < 0)
		|| (_x >= m_width)
		|| (_y < 0)
		|| (_y >= m_height)
		)
		return PART_NONE;

	return m_pParts[_y][_x];
}

int CourseManager::getParts(glm::vec2 const& _point)
{
	ivec2 cellPoint = (ivec2)_point / PART_SIZE;
	if (
		(cellPoint.x < 0)
		|| (cellPoint.x >= m_width)
		|| (cellPoint.y < 0)
		|| (cellPoint.y >= m_height)
		)
		return PART_NONE;

	return m_pParts[cellPoint.y][cellPoint.x];
}

bool CourseManager::intersect(vec2 const& _point) {
	ivec2 cellPoint = (ivec2)_point / PART_SIZE;
	if (
		(cellPoint.x < 0)
		|| (cellPoint.x >= m_width)
		|| (cellPoint.y < 0)
		|| (cellPoint.y >= m_height)
		)
		return false;

	switch (m_pParts[cellPoint.y][cellPoint.x]) {
	case PART_GROUND:
	case PART_HARD_BLOCK:
	case PART_SOFT_BLOCK:
	case PART_PIPE_UP_LEFT:
	case PART_PIPE_UP_RIGHT:
	case PART_PIPE_DOWN_LEFT:
	case PART_PIPE_DOWN_RIGHT:
	case PART_QUESTION0:
	case PART_QUESTION1:
	case PART_QUESTION2:
	case PART_QUESTION3:
	case PART_GROUND_2:
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
	int scrolleColumn = (int)m_scroll / PART_SIZE;
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
		g_pSound->play(SOUND_SE_BUMP);
		break;
	default:
		g_pSound->play(SOUND_SE_BUMP);
		return;
	}

	setParts(cellPoint, PART_NONE);
	m_hitBlockController.start(cellPoint, (PART)part, (PART)endPart);
}

bool CourseManager::getClearAex(Rect& _rect)
{
	if (m_clearAex.m_position != vec2(0, 0)) {
		_rect = m_clearAex;
		return true;
	}
	return false;
}
