#include <stdio.h>
#include "Part.h"

#include "glut.h"

using namespace glm;

Part g_parts[PART_MAX] = {
	 {nullptr,}, // PART_NONE
	 {"b0.bmp",},// PART_GROUND
	 {"b1.bmp",},// PART_HARD_BLOCK
	 {"b2.bmp",},// PART_SOFT_BLOCK
	 {"p0.bmp",},// PART_PIPE_UP_LEFT
	 {"p1.bmp",},// PART_PIPE_UP_RIGHT
	 {"p2.bmp",},// PART_PIPE_DOWN_LEFT
	 {"p3.bmp",},// PART_PIPE_DOWN_RIGHT
	 {"q0.bmp",},// PART_QUESTION0
	 {"q1.bmp",},// PART_QUESTION1
	 {"q2.bmp",},// PART_QUESTION2
	 {"q3.bmp",},// PART_QUESTION3
	 {"c0.bmp",},// PART_CLOUD_UP_LEFT
	 {"c1.bmp",},// PART_CLOUD_UP
	 {"c2.bmp",},// PART_CLOUD_UP_RIGHT
	 {"c3.bmp",},// PART_CLOUD_DOWN_LEFT
	 {"c4.bmp",},// PART_CLOUD_DOWN
	 {"c5.bmp",},// PART_CLOUD_DOWN_RIGHT
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
	 {"f0.bmp",},// PART_FORT_TOP
	 {"f1.bmp",},// PART_FORT_LEFT
	 {"f2.bmp",},// PART_FORT_WALL
	 {"f3.bmp",},// PART_FORT_RIGHT
	 {"f4.bmp",},// PART_FORT_MIDDLE
	 {"f5.bmp",},// PART_FORT_GATE_UP
	 {"f6.bmp",},// PART_FORT_GATE_DOWN
	 {"b3.bmp",}, // PART_BLOCK,
	 {"s0.bmp",},// PART_SEA_0
	 {"s1.bmp",},// PART_SEA_1
	 {"s2.bmp",},// PART_SEA_2
	 {"s3.bmp",},// PART_SEA_3
	 {"s4.bmp",},// PART_SEA_4
	 {"s5.bmp",},// PART_SEA_5
	 {"s6.bmp",},// PART_SEA_6
	 {"s7.bmp",},// PART_SEA_7
	 {"d0.bmp",},// PART_DESERT_0
	 {"d1.bmp",}, // PART_DESERT_1,
	 {"d2.bmp",}, // PART_DESERT_2,
	 {"d3.bmp",}, // PART_DESERT_3,
	 {"d4.bmp",}, // PART_DESERT_4,
	 {"d5.bmp",}, // PART_DESERT_5,
	 {"d6.bmp",}, // PART_DESERT_6,
	 {"d7.bmp",}, // PART_DESERT_7,
	 {"d8.bmp",}, // PART_DESERT_8,
};

int Part::init() {
//	if (m_fileName != nullptr) {
//		glGenTextures(
//			1,				// GLsizei n
//			&m_texture);	// GLuint * textures
//
//		glBindTexture(
//			GL_TEXTURE_2D,	// GLenum target
//			m_texture);		// GLuint texture
//		char fileName[256] = "resource\\textures\\parts\\";
//		sprintf_s(fileName, "%s%s", fileName, m_fileName);
//		//printf("%s\n", fileName);
//		if (texFromBMP(fileName, 0xff, 0x00, 0xff) != 0) {
//#if _DEBUG
//			printf("The file %s was open failed\n", fileName);
//#endif // _DEBUG
//			return 1;
//		}
//	}
	return 0;
}

int Part::initAll() {
	int failedCount = 0;
	for (int i = 0; i < PART_MAX; i++) {
		//if (g_parts[i].init() != 0) {
		//	failedCount++;
		//}


		int x = i % PART_SIZE;
		int y = i / PART_SIZE;

		g_parts[i].m_uvX = x * PART_SIZE;
		g_parts[i].m_uvY = y * PART_SIZE;
		g_parts[i].m_sizeX = g_parts[i].m_uvX + PART_SIZE;
		g_parts[i].m_sizeY = g_parts[i].m_uvY + PART_SIZE;
	}

	//if (failedCount > 0) {
	//	return 1;
	//}
	return 0;
}
