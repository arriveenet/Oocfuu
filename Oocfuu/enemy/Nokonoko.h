#pragma once
#include "Sprite.h"

#include <glm/glm.hpp>
#include <vector>

enum NOKONOKO_STATE {
	NOKONOKO_STATE_MOVE,
	NOKONOKO_STATE_SQUISH,
	NOKONOKO_STATE_SPIN,
	NOKONOKO_STATE_RETURN,
	NOKONOKO_STATE_DEAD,
	NOKONOKO_STATE_MAX
};

class Nokonoko : public Sprite{
	bool m_dead;
	bool m_falling;
	NOKONOKO_STATE m_state;
	int m_counter;
	glm::vec2 m_speed;
	glm::vec2 m_rightPoint;
	glm::vec2 m_leftPoint;
	glm::vec2 m_topPoint;
	std::vector<glm::vec2> m_bottomPoints;

public:
	Nokonoko();
	Nokonoko(glm::vec2 _position);
	Nokonoko(float _x, float _y);

	void update();
	void draw();
};