#pragma once

#define PART_SIZE	16

enum {
	PART_NONE,
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
	PART_MAX
};

struct Part{
	const char* m_fileName;
	unsigned int m_texture;

	int init();
	static int initAll();
};

extern Part g_parts[PART_MAX];