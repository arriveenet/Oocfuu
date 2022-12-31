#pragma once
#include "Rect.h"

#define LIFT_SIZE			8
#define LIFT_DISTANCE		40
#define LIFT_SPEED			0.8f

enum LIFTMODE {
	LIFTMODE_LEFTANDRIGHT,
	LIFTMODE_UPANDDOWN,
	LIFTMODE_MAX
};

class Lift : public Rect{
	int m_width;
	LIFTMODE m_mode;
	glm::vec2 m_speed;
	glm::vec2 m_startPosition;

public:
	Lift(float _x, float _y, int _width, LIFTMODE _mode);

	void update();
	void draw();
	bool intersect(glm::vec2 const& _point);
};

