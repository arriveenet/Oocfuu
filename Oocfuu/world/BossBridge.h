#pragma once
#include "Rect.h"
#include <glm/glm.hpp>
#include <vector>

class BossBridge {
	std::vector<Rect> m_parts;
	glm::ivec2 m_ChainPosition;
	bool m_destroy;
	bool m_exist;

public:
	BossBridge();
	~BossBridge();

	void update();
	void draw();

	void clear();
	void add(float _x, float _y);
	void setChain(int _x, int _y);
	void destroy();
	bool isDestroyed();
};