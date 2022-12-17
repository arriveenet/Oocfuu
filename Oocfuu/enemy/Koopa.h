#pragma once
#include "Sprite.h"

#include <vector>
#include <glm/glm.hpp>

#define KOOPA_WIDTH			32
#define KOOPA_HEIGHT		32
#define KOOPA_FALL_SPEED	0.3f

enum KOOPAP_STATE {
	KOOPAP_STATE_IDLE,
	KOOPAP_STATE_FRONTJUMP,
	KOOPAP_STATE_BACKJUMP,
	KOOPAP_STATE_BREATH,
	KOOPAP_STATE_MAX
};

class Koopa : public Sprite {
	bool m_dead;
	bool m_falling;
	KOOPAP_STATE m_state;
	int m_counter;
	glm::vec2 m_speed;
	std::vector<glm::vec2> m_bottomPoints;

public:
	Koopa();
	Koopa(glm::vec2 _position);
	Koopa(float _x, float _y);
	~Koopa();

	void update() override;
	void draw() override;
};