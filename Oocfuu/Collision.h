#pragma once
#include "Rect.h"

#include <vector>
#include <functional>
#include <glm/glm.hpp>

class Collision {
public:
	std::vector<glm::vec2> m_leftPoints;
	std::vector<glm::vec2> m_rightPoints;
	std::vector<glm::vec2> m_topPoints;
	std::vector<glm::vec2> m_bottomPoints;

	void leftCollision(Rect _rect, std::function<void()> _proc);
	void rightCollision(Rect _rect, std::function<void()> _proc);
	void topCollision(Rect _rect, std::function<void()> _proc);
	void bottomCollision(Rect _rect, std::function<void()> _proc);

	void clear();
};

