#include "CourseManager.h"

#include "App.h"
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

using namespace glm;
using namespace std;

CourseManager g_courseManager;

CourseManager::CourseManager()
	: m_scroll(0.0f)
	, m_isLoaded(false)
	, m_kinopio()
	, m_currentCourse()
{
}

CourseManager::~CourseManager()
{
	release();
	clear();
}

void CourseManager::release()
{
	// コースが読み込まれていた場合、破棄する
	if (m_currentCourse.m_isLoaded) {
		m_currentCourse.destroy();
	}
}

void CourseManager::clear()
{
	// コースエフェクトマネージャーをクリアする
	CourseEffectManager* courseEffectMgr = CourseEffectManager::instance();
	courseEffectMgr->clear();

	// キノピオを無効する
	m_kinopio.m_enable = false;

	// 仕掛けパーツをクリアする
	g_gmmickPart.clear();

	// 敵キャラクターをクリアする
	g_enemyManager.clear();
}

void CourseManager::update()
{
	// ブロックを叩いたときのコインを更新
	CourseEffectManager* courseEffectMgr = CourseEffectManager::instance();
	courseEffectMgr->update();

	m_currentCourse.update();
}

void CourseManager::draw()
{
	// コースを描画
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
	case PART_SOFT_BLOCK:
	case PART_HARD_BLOCK_1:
	case PART_HARD_BLOCK_2:
	case PART_HARD_BLOCK_3:
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
	case PART_WOOD_1:
	case PART_WOOD_2:
	case PART_WOOD_3:
	case PART_BRIDGE:
		return true;
	}
	return false;
}

void CourseManager::intersectCoin(Player* _pPlayer)
{
	//int scrolleColumn = (int)m_scroll / PART_SIZE;
	vector<ivec2>::iterator iter = m_currentCourse.m_coins.begin();
	for (; iter != m_currentCourse.m_coins.end(); ++iter) {
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
	default:
		g_pSound->play(SOUND_TYPE_SE_BUMP);
		return;
	}

	setParts(cellPoint, PART_NONE);
	m_currentCourse.m_hitBlockController.start(cellPoint, (PART)part, (PART)endPart);

	g_enemyManager.hitBlock(cellPoint);
}

bool CourseManager::getClearAex(Rect& _rect)
{
	if (m_currentCourse.m_clearAex.m_position != vec2(0, 0)) {
		_rect = m_currentCourse.m_clearAex;
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
	if (m_currentCourse.m_isLoaded) {
		m_currentCourse.destroy();
	}

	m_currentCourse = *pCourse;
}
