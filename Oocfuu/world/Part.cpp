#include <stdio.h>
#include "Part.h"
#include "TextureManager.h"

#include <gl/glut.h>

using namespace glm;

Part g_parts[PART_MAX] = {
	// {nullptr,}, // PART_NONE
	 {"b0.bmp",},// PART_GROUND
	 {"b1.bmp",},// PART_SOFT_BLOCK
	 {"b2.bmp",},// PART_HARD_BLOCK_1
	 {"b3.bmp",},// PART_HARD_BLOCK_2
	 {"b4.bmp",},// PART_HARD_BLOCK_3
	 {"f0.bmp",},// PART_FORT_TOP
	 {"f1.bmp",},// PART_FORT_LEFT
	 {"f2.bmp",},// PART_FORT_WALL
	 {"f3.bmp",},// PART_FORT_RIGHT
	 {"f4.bmp",},// PART_FORT_MIDDLE
	 {"f5.bmp",},// PART_FORT_GATE_UP
	 {"f6.bmp",},// PART_FORT_GATE_DOWN
	 {"w0.bmp",},// PART_WOOD_0
	 {"w1.bmp",},// PART_WOOD_1
	 {"w2.bmp",},// PART_WOOD_2
	 {"w3.bmp",},// PART_WOOD_3
	 {"p4.bmp",},// PART_SIDE_PIPE_UP_LEFT
	 {"p5.bmp",},// PART_SIDE_PIPE_DOWN_LEFT
	 {"p6.bmp",},// PART_SIDE_PIPE_UP
	 {"p7.bmp",},// PART_SIDE_PIPE_DOWN
	 {"p8.bmp",},// PART_SIDE_PIPE_JOINT_UP
	 {"p9.bmp",},// PART_SIDE_PIPE_JOINT_DOWN
	 {"p0.bmp",},// PART_PIPE_UP_LEFT
	 {"p1.bmp",},// PART_PIPE_UP_RIGHT
	 {"p2.bmp",},// PART_PIPE_DOWN_LEFT
	 {"p3.bmp",},// PART_PIPE_DOWN_RIGHT
	 {"t0.bmp",},// PART_TREE_LEFT
	 {"t1.bmp",},// PART_TREE_CENTER
	 {"t2.bmp",},// PART_TREE_RIGHT
	 {"m0.bmp",},// PART_MOUNTAIN_TOP
	 {"m1.bmp",},// PART_MOUNTAIN_LEFT
	 {"m2.bmp",},// PART_MOUNTAIN_CENTER0
	 {"m3.bmp",},// PART_MOUNTAIN_RIGHT
	 {"m4.bmp",},// PART_MOUNTAIN_PLANE
	 {"m5.bmp",},// PART_MOUNTAIN_CENTER1
	 {"g0.bmp",},// PART_GOAL_TOP
	 {"g1.bmp",},// PART_GOAL_POLE
	 {"cn.bmp",},// PART_CHAIN
	 {"q0.bmp",},// PART_QUESTION0
	 {"q1.bmp",},// PART_QUESTION1
	 {"q2.bmp",},// PART_QUESTION2
	 {"q3.bmp",},// PART_QUESTION3
	 {"a0.bmp",},// PART_AXE_0
	 {"a1.bmp",},// PART_AXE_1
	 {"a2.bmp",},// PART_AXE_2
	 {"e0.bmp",},// PART_COIN_0
	 {"e1.bmp",},// PART_COIN_1
	 {"e2.bmp",},// PART_COIN_2
	 {"br.bmp",},// PART_BRIDGE
	 {"mg.bmp",},// PART_MAGMA_0
	 {"mh.bmp",},// PART_MAGMA_1
	 {"c0.bmp",},// PART_CLOUD_UP_LEFT
	 {"c1.bmp",},// PART_CLOUD_UP
	 {"c2.bmp",},// PART_CLOUD_UP_RIGHT
	 {"c3.bmp",},// PART_CLOUD_DOWN_LEFT
	 {"c4.bmp",},// PART_CLOUD_DOWN
	 {"c5.bmp",},// PART_CLOUD_DOWN_RIGHT
	 {"s0.bmp",},// PART_SEA_0
	 {"s1.bmp",},// PART_SEA_1
	 {"s2.bmp",},// PART_SEA_2
	 {"s3.bmp",},// PART_SEA_3
	 {"s4.bmp",},// PART_SEA_4
	 {"s5.bmp",},// PART_SEA_5
	 {"s6.bmp",},// PART_SEA_6
	 {"s7.bmp",},// PART_SEA_7
	 {"d0.bmp",},// PART_DESERT_0
	 {"d1.bmp",},// PART_DESERT_1,
	 {"d2.bmp",},// PART_DESERT_2,
	 {"d3.bmp",},// PART_DESERT_3,
	 {"d4.bmp",},// PART_DESERT_4,
	 {"d5.bmp",},// PART_DESERT_5,
	 {"d6.bmp",},// PART_DESERT_6,
	 {"d7.bmp",},// PART_DESERT_7,
	 {"d8.bmp",},// PART_DESERT_8,
};

PartManager g_partManager;

/** コンストラクタ
*/
PartManager::PartManager()
{
}

/** コンストラクタ
*/
PartManager::~PartManager()
{
}


/** パーツのテクスチャ座標を初期化する
* @param[out] 成功:true、失敗:false
*/
bool PartManager::init()
{
	vec2 textureSize = g_textureManager.getSize(TEXTURE_PARTS_OVERWORLD);

	// テクスチャのサイズがゼロベクトルか判定する
	if (length(textureSize) == 0) {
		return false;
	}

	for (int i = 0; i < PART_MAX; i++) {
		int x = i % PART_SIZE;
		int y = i / PART_SIZE;

		ivec2 position = { x * PART_SIZE, y * PART_SIZE };
		ivec2 size = position + PART_SIZE;

		vec2 topLeft = static_cast<vec2>(position) / textureSize.x;
		vec2 bottomRight = static_cast<vec2>(size) / textureSize.y;

		g_parts[i].m_texCoords[0] = topLeft;
		g_parts[i].m_texCoords[1] = {topLeft.x, bottomRight.y};
		g_parts[i].m_texCoords[2] = bottomRight;
		g_parts[i].m_texCoords[3] = {bottomRight.x, topLeft.y};
	}

	return true;
}

/** テクスチャ座標の配列の0番目のポインタを取得
* @param[in] _index パーツの種類
* @param[out] テクスチャ座標のポインタ
*/
vec2* PartManager::getTexCoords(int _index)
{
	assert(_index >= 0 && _index < PART_MAX);

	return &g_parts[_index].m_texCoords[0];
}
