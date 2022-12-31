#include "GimmickPart.h"

using namespace std;

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
		Firebar* pFirebar = &(*itr);
		pFirebar->upadte();
	}

	for (vector<Lift>::iterator itr = m_lifts.begin();
		itr != m_lifts.end();
		itr++) {
		Lift* pLift = &(*itr);
		pLift->update();
	}
}

void GimmickPart::draw()
{
	for (vector<Firebar>::iterator itr = m_firebars.begin();
		itr != m_firebars.end();
		itr++) {
		Firebar* pFirebar = &(*itr);
		pFirebar->draw();
	}

	for (vector<Lift>::iterator itr = m_lifts.begin();
		itr != m_lifts.end();
		itr++) {
		Lift* pLift = &(*itr);
		pLift->draw();
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

bool GimmickPart::intersectLift(const glm::vec2& _point)
{
	for (vector<Lift>::iterator itr = m_lifts.begin();
		itr != m_lifts.end();
		itr++) {
		Lift* pLift = &(*itr);
		if (pLift->intersect(_point))
			return true;
	}
	return false;
}
