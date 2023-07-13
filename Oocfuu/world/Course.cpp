#include "Course.h"

/**
 * @brief �f�t�H���g�R���X�g���N�^
 * 
 */
Course::Course()
	: m_width(0)
	, m_height(0)
	, m_pParts(nullptr)
	, m_clearColor(0L)
	, m_startPosition(0, 0)
	, m_nexstWorld()
{
}

/**
 * @brief �f�X�g���N�^
 *
 */
Course::~Course()
{
}

void Course::clear()
{
	if (m_pParts) {
		for (int i = 0; i < m_height; ++i) {
			delete m_pParts[i];
		}
		delete[] m_pParts;
	}
}
