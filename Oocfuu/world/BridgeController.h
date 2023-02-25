#pragma once
#include "common/range.h"
#include <glm/glm.hpp>
#include <vector>

class BridgeController {
private:
	std::vector<glm::ivec2> m_parts;
	glm::ivec2 m_ChainPosition;
	bool m_destroy;
	bool m_exist;

public:
	BridgeController();
	virtual ~BridgeController();

	void update();

	void clear();
	void add(int _x, int _y);
	void setChain(int _x, int _y);
	void destroy();
	bool isDestroyed();
	RANGE getRange();
};