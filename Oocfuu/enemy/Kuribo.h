#pragma once
#include "Sprite.h"

#include <glm/glm.hpp>
#include <vector>

#define KURIBO_WIDTH			16
#define KURIBO_HEIGHT			16

#define KURIBO_SPEED			0.5f
#define KUROBO_FALL_SPEED		0.3f

#define KURIBO_TOP_POINT_COUNT	3

enum {
	KURIBO_STATE_MOVE,
	KURIBO_STATE_SQUISH,
	KURIBO_STATE_DEAD,
	KURIBO_STATE_MAX
};

class Kuribo : public Sprite{
	bool m_dead;
	bool m_falling;
	int m_state;
	int m_counter;
	glm::vec2 m_speed;
	glm::vec2 m_rightPoint;
	glm::vec2 m_leftPoint;
	glm::vec2 m_topPoints[KURIBO_TOP_POINT_COUNT];
	std::vector<glm::vec2> m_bottomPoints;

public:
	Kuribo();
	Kuribo(glm::vec2 _position);
	Kuribo(float _x, float _y);

	void update() override;
	void draw() override;
	void turn();
	Rect getRect() const;
};