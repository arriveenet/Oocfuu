#include "BridgeController.h"
#include "Game.h"
#include "Part.h"
#include "CourseManager.h"
#include "TextureManager.h"
#include "sound/Sound.h"

using namespace std;
using namespace glm;

BridgeController::BridgeController()
	: m_ChainPosition(0, 0)
	, m_destroy(false)
	, m_exist(false)
{
}

BridgeController::~BridgeController()
{
}

void BridgeController::update()
{
	if (m_destroy && !m_parts.empty()) {
		if (Game::m_count % 4 == 0) {
			ivec2 position = m_parts.back();
			g_courseManager.setParts(position, PART_NONE);
			m_parts.pop_back();
			g_pSound->play(SOUND_TYPE_SE_BREAK);
		}
	}
}

void BridgeController::clear()
{
	m_parts.clear();
	m_destroy = false;
	m_exist = false;
}

void BridgeController::add(int _x, int _y)
{
	m_exist = true;
	m_parts.push_back(ivec2(_x, _y));
}

void BridgeController::setChain(int _x, int _y)
{
	m_ChainPosition = { _x, _y };
}

void BridgeController::destroy()
{
	m_destroy = true;
	g_courseManager.setParts(m_ChainPosition, PART_NONE);
}

bool BridgeController::isDestroyed()
{
	return m_exist && m_parts.empty();
}

RANGE BridgeController::getRange()
{
	RANGE range = { 0.0f };

	if (!m_exist) {
		return range;
	}

	ivec2 front = m_parts.front();
	ivec2 back = m_parts.back();

	range.start = (float)front.x * PART_SIZE;
	range.end = (float)back.x * PART_SIZE + PART_SIZE;

	return range;
}
