#pragma once

#include <glm/glm.hpp>

#define PART_SIZE	16

enum PART {
	PART_NONE = -1,
	PART_GROUND,
	PART_HARD_BLOCK,
	PART_SOFT_BLOCK,
	PART_PIPE_UP_LEFT,
	PART_PIPE_UP_RIGHT,
	PART_PIPE_DOWN_LEFT,
	PART_PIPE_DOWN_RIGHT,
	PART_QUESTION0,
	PART_QUESTION1,
	PART_QUESTION2,
	PART_QUESTION3,
	PART_CLOUD_UP_LEFT,
	PART_CLOUD_UP,
	PART_CLOUD_UP_RIGHT,
	PART_CLOUD_DOWN_LEFT,
	PART_CLOUD_DOWN,
	PART_CLOUD_DOWN_RIGHT,
	PART_TREE_LEFT,
	PART_TREE_CENTER,
	PART_TREE_RIGHT,
	PART_MOUNTAIN_TOP,
	PART_MOUNTAIN_LEFT,
	PART_MOUNTAIN_CENTER0,
	PART_MOUNTAIN_RIGHT,
	PART_MOUNTAIN_PLANE,
	PART_MOUNTAIN_CENTER1,
	PART_GOAL_TOP,
	PART_GOAL_POLE,
	PART_FORT_TOP,
	PART_FORT_LEFT,
	PART_FORT_WALL,
	PART_FORT_RIGHT,
	PART_FORT_MIDDLE,
	PART_FORT_GATE_UP,
	PART_FORT_GATE_DOWN,
	PART_GROUND_2,
	PART_SEA_0,
	PART_SEA_1,
	PART_SEA_2,
	PART_SEA_3,
	PART_SEA_4,
	PART_SEA_5,
	PART_SEA_6,
	PART_SEA_7,
	PART_DESERT_0,
	PART_DESERT_1,
	PART_DESERT_2,
	PART_DESERT_3,
	PART_DESERT_4,
	PART_DESERT_5,
	PART_DESERT_6,
	PART_DESERT_7,
	PART_DESERT_8,
	PART_WOOD_0,
	PART_WOOD_1,
	PART_WOOD_2,
	PART_WOOD_3,
	PART_BRIDGE,
	PART_MAGMA_0,
	PART_MAGMA_1,
	PART_AXE_0,
	PART_AXE_1,
	PART_AXE_2,
	PART_CHAIN,
	PART_COIN_0,
	PART_COIN_1,
	PART_COIN_2,
	PART_MAX
};

// パート構造体
struct Part {
	const char* m_fileName;
	unsigned int m_texture;
	glm::vec2 m_texCoords[4];
};

// パーツ管理クラス
class PartManager {
public:
	PartManager();
	virtual ~PartManager();

	bool init();
	glm::vec2* getTexCoords(int _index);

};

extern Part g_parts[PART_MAX];
extern PartManager g_partManager;