#include "GimmickPart.h"

using namespace std;
using namespace glm;

GimmickPart g_gmmickPart;

GimmickPart::GimmickPart()
{
}

GimmickPart::~GimmickPart()
{
}


void GimmickPart::update()
{
	for (vector<Firebar>::iterator itr = m_firebars.begin();
		itr != m_firebars.end();
		itr++) {
		itr->upadte();
	}

	for (vector<Lift>::iterator itr = m_lifts.begin();
		itr != m_lifts.end();
		itr++) {
		itr->update();
	}
}

void GimmickPart::draw()
{
	for (vector<Firebar>::iterator itr = m_firebars.begin();
		itr != m_firebars.end();
		itr++) {
		itr->draw();
	}

	for (vector<Lift>::iterator itr = m_lifts.begin();
		itr != m_lifts.end();
		itr++) {
		itr->draw();
	}
}

void GimmickPart::clear()
{
	m_firebars.clear();
	m_lifts.clear();
}

void GimmickPart::addFirebar(const Firebar& _firebar)
{
	m_firebars.push_back(_firebar);
}

void GimmickPart::addLift(const Lift& _lift)
{
	m_lifts.push_back(_lift);
}

bool GimmickPart::intersectLift(const vec2& _point, vec2& _liftPosition, vec2& _liftSpeed)
{
	vector<Lift>::iterator itr = m_lifts.begin();
	for (; itr != m_lifts.end(); itr++) {
		if (itr->intersect(_point)) {
			_liftPosition = itr->m_position;
			_liftSpeed = itr->getSpeed();
			return true;
		}
	}
	return false;
}
