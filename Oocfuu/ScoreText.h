#pragma once
#include "Rect.h"
#include "TextureManager.h"

#include <vector>

#define SCORE_TEXT_WIDTH	16
#define SCORE_TEXT_HEIHGT	8

enum SCORETYPE {
	SCORETYPE_100,
	SCORETYPE_200,
	SCORETYPE_400,
	SCORETYPE_500,
	SCORETYPE_800,
	SCORETYPE_1000,
	SCORETYPE_2000,
	SCORETYPE_4000,
	SCORETYPE_5000,
	SCORETYPE_8000,
	SCORETYPE_MAX
};

class ScoreText : public Rect{
private:
	SCORETYPE m_scoreType;
	TEXTURE m_texture;
	unsigned int m_counter;
	bool m_visible;

public:
	ScoreText(SCORETYPE _type, float _x, float _y);
	~ScoreText();
	
	void update();
	void draw();
	bool isVisible();

protected:
	void setTexture();
};

class ScoreTextManager {
private:
	std::vector<ScoreText> m_scoreTexts;

public:
	ScoreTextManager();
	~ScoreTextManager();

	void update();
	void draw();
	void add(SCORETYPE _type, float _x, float _y);
};

extern ScoreTextManager g_scoreTextManager;