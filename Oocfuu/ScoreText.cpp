#include "ScoreText.h"
#include "Game.h"

#include <glm/glm.hpp>

using namespace std;
using namespace glm;

ScoreTextManager g_scoreTextManager;

ScoreText::ScoreText(SCORETYPE _type, float _x, float _y)
	: m_scoreType(_type)
	, m_counter(0)
	, m_visible(true)
	, Rect(vec2(SCORE_TEXT_WIDTH, SCORE_TEXT_HEIHGT), vec2(_x, _y))
{
	setTexture();
}

ScoreText::~ScoreText()
{
}

void ScoreText::update()
{
	if (++m_counter < 60 * 4) {
		m_position.y -= 0.2f;
		printf("m_counter=%d\n", m_counter);
	} else
		m_visible = false;
}

void ScoreText::draw()
{
	if (!m_visible)
		return;

	g_textureManager.setTexture(m_texture);
	Rect::draw();
	g_textureManager.unbindTexture();
}

bool ScoreText::isVisible()
{
	return m_visible;
}

void ScoreText::setTexture()
{
	switch (m_scoreType) {
	case SCORETYPE_100:
		m_texture = TEXTURE_SCORE_100;
		break;
	case SCORETYPE_200:
		m_texture = TEXTURE_SCORE_100;
		break;
	case SCORETYPE_400:
		m_texture = TEXTURE_SCORE_100;
		break;
	case SCORETYPE_500:
		m_texture = TEXTURE_SCORE_100;
		break;
	case SCORETYPE_800:
		m_texture = TEXTURE_SCORE_100;
		break;
	case SCORETYPE_1000:
		m_texture = TEXTURE_SCORE_100;
		break;
	case SCORETYPE_2000:
		m_texture = TEXTURE_SCORE_100;
		break;
	case SCORETYPE_4000:
		m_texture = TEXTURE_SCORE_100;
		break;
	case SCORETYPE_5000:
		m_texture = TEXTURE_SCORE_100;
		break;
	case SCORETYPE_8000:
		m_texture = TEXTURE_SCORE_100;
		break;
	default:
		break;
	}
}

ScoreTextManager::ScoreTextManager()
{
}

ScoreTextManager::~ScoreTextManager()
{
}

void ScoreTextManager::update()
{
	vector<ScoreText>::iterator iter = m_scoreTexts.begin();
	for (; iter != m_scoreTexts.end(); iter++) {
		iter->update();

		// ”ñ•\Ž¦‚É‚È‚Á‚½ê‡íœ‚·‚é
		if (!iter->isVisible())
			m_scoreTexts.erase(iter);
	}
}

void ScoreTextManager::draw()
{
	vector<ScoreText>::iterator iter = m_scoreTexts.begin();
	for (; iter != m_scoreTexts.end(); iter++) {
		iter->draw();
	}
}

void ScoreTextManager::add(SCORETYPE _type, float _x, float _y)
{
	m_scoreTexts.push_back(ScoreText(_type, _x, _y));
}
