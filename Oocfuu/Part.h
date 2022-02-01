#pragma once

#define PART_SIZE	16

enum {
	PART_NONE,
	PART_GROUND,
	PART_HARD_BLOCK,
	PART_HARD_BLOCK_2,
	PART_SOFT_BLOCK,
	PART_MAX
};

struct Part{
	const char* m_fileName;
	unsigned int m_texture;

	int init();
	static int initAll();
};

extern Part g_parts[PART_MAX];