#include "Course.h"

/**
 * @brief デフォルトコンストラクタ
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
 * @brief デストラクタ
 *
 */
Course::~Course()
{
}
