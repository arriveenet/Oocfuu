#pragma once
#include "Rect.h"
#include "Course.h"

#include <vector>
#include <functional>
#include <glm/glm.hpp>

class Collision {
private:
	Rect m_rect;

	std::vector<glm::vec2> m_leftPoints;
	std::vector<glm::vec2> m_rightPoints;
	std::vector<glm::vec2> m_topPoints;
	std::vector<glm::vec2> m_bottomPoints;

public:
	Collision();

	void clear();
	void prepare(const Rect& _rect);

	template<typename TUpdate>
	inline void updateLeft(TUpdate _update, CourseManager _course);

	template<typename TUpdate>
	inline void updateRight(TUpdate _update, CourseManager _course);

	 template<typename TUpdate>
	 inline void updateTop(TUpdate _update, CourseManager _course);

	 template<typename TUpdate>
	 inline void updateBottom(TUpdate _update, CourseManager _course);

	 void drawPoints();
};

