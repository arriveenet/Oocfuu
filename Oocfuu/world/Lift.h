#pragma once
#include "Rect.h"

#define LIFT_SIZE			8
#define LIFT_DISTANCE		40
#define LIFT_SPEED			0.8f

enum LIFT_MOVEMENT {
	LIFT_LEFT_AND_RIGHT,
	LIFT_UP_AND_DOWN,
	LEFT_SCREEN_UP,
	LEFT_SCREEN_DOWN,
	LIFT_MOVEMENT_MAX
};

class Lift : public Rect{
	int m_width;
	int m_distance;
	LIFT_MOVEMENT m_mode;
	glm::vec2 m_speed;
	glm::vec2 m_startPosition;

public:
	Lift(float _x, float _y, int _width, LIFT_MOVEMENT _mode, int _distance = LIFT_DISTANCE);

	void update();
	void draw();
	bool intersect(glm::vec2 const& _point);
	glm::vec2 getSpeed() const { return m_speed; };
};

