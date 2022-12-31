#pragma once
#include "Firebar.h"
#include "Lift.h"

#include <vector>

enum GIMICKPART {
	GIMICKPART_FIREBAR,
	GIMICKPART_LIFT,
	GIMICKPART_MAX
};


class GimmickPart {
	std::vector<Firebar> m_firebars;
	std::vector<Lift> m_lifts;

public:
	GimmickPart();
	~GimmickPart();
	
	void update();
	void draw();
	void clear();
	void addFirebar(const Firebar& _firebar);
	void addLift(const Lift& _lift);
	bool intersectLift(const glm::vec2& _point);
};

extern GimmickPart g_gmmickPart;
