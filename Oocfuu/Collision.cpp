#include "Collision.h"
#include "Part.h"
#include "Course.h"

using namespace glm;
using namespace std;

void Collision::leftCollision(Rect _rect, std::function<void()> _proc)
{
	for (vector<vec2>::iterator iter = m_leftPoints.begin();
		iter != m_leftPoints.end();
		iter++) {
		if (g_courseManager.intersect(*iter)) {
			vec2 left = (ivec2)*iter / PART_SIZE * PART_SIZE;
			_rect.m_position.x = left.x + _rect.m_size.x;

			_proc();
			break;
		}
	}
}

void Collision::rightCollision(Rect _rect, std::function<void()> _proc)
{
	for (vector<vec2>::iterator iter = m_rightPoints.begin();
		iter != m_rightPoints.end();
		iter++) {
		if (g_courseManager.intersect(*iter)) {
			vec2 left = (ivec2)*iter / PART_SIZE * PART_SIZE;
			_rect.m_position.x = left.x - _rect.m_size.x;

			_proc();
			break;
		}
	}
}

void Collision::topCollision(Rect _rect, std::function<void()> _proc)
{
	for (vector<vec2>::iterator iter = m_topPoints.begin();
		iter != m_topPoints.end();
		iter++) {
		if (g_courseManager.intersect(*iter)) {
			vec2 left = (ivec2)*iter / PART_SIZE * PART_SIZE;
			_rect.m_position.x = left.x + _rect.m_size.x;

			_proc();
			break;
		}
	}
}

void Collision::bottomCollision(Rect _rect, std::function<void()> _proc)
{
	for (vector<vec2>::iterator iter = m_bottomPoints.begin();
		iter != m_bottomPoints.end();
		iter++) {
		if (g_courseManager.intersect(*iter)) {
			vec2 left = (ivec2)*iter / PART_SIZE * PART_SIZE;
			_rect.m_position.x = left.x + _rect.m_size.x;

			_proc();
			break;
		}
	}
}

void Collision::clear()
{
	m_leftPoints.clear();
	m_rightPoints.clear();
	m_topPoints.clear();
	m_bottomPoints.clear();
}
