#include "BossBridge.h"
#include "Game.h"
#include "Part.h"
#include "Course.h"
#include "TextureManager.h"
#include "sound/Sound.h"

using namespace std;
using namespace glm;

BossBridge::BossBridge()
	: m_ChainPosition(0, 0)
	, m_destroy(false)
	, m_exist(false)
{
}

BossBridge::~BossBridge()
{
}

void BossBridge::update()
{
	if (m_destroy && !m_parts.empty()) {
		if (Game::m_count % 4 == 0) {
			m_parts.pop_back();
			g_pSound->play(SOUND_SE_BREAK);
		}
	}
}

void BossBridge::draw()
{
	if (m_parts.empty())
		return;

	g_textureManager.setTexture(TEXTURE_BOSSBRIDGE);
	vector<Rect>::iterator iter = m_parts.begin();
	for (; iter != m_parts.end(); iter++) {
		iter->draw();
	}
	g_textureManager.unbindTexture();
}

void BossBridge::clear()
{
	m_parts.clear();
	m_destroy = false;
	m_exist = false;
}

void BossBridge::add(float _x, float _y)
{
	m_exist = true;
	m_parts.push_back(Rect(vec2(PART_SIZE, PART_SIZE), vec2(_x, _y)));
}

void BossBridge::setChain(int _x, int _y)
{
	m_ChainPosition = { _x, _y };
}

void BossBridge::destroy()
{
	m_destroy = true;
	g_courseManager.setParts(m_ChainPosition, PART_NONE);
}

bool BossBridge::isDestroyed()
{
	return m_exist && m_parts.empty();
}
