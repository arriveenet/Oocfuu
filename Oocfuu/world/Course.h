#pragma once
#include <Windows.h>
#include <glm/glm.hpp>
#include "Game.h"
#include "CourseManager.h"
#include "CourseLoader.h"

/**
 * @breif コースクラス
 * 
 */
class Course {
public:
	int			m_width;			//!< コースの幅
	int			m_height;			//!< コースの高さ
	int**		m_pParts;			//!< パーツの2次元配列
	COLORREF	m_clearColor;		//!< 背景色
	glm::vec2	m_startPosition;	//!< プレイヤーのスタート位置
	WORLD		m_nexstWorld;		//!< 次のワールド


	// コンストラクタ
	Course();

	// デストラクタ
	virtual ~Course();

	void clear();

	// コースの幅を取得
	int getWidth() const noexcept;

	// コースの高さを取得
	int getHeight() const noexcept;

	int** getParts() const noexcept;
};

/**
 * @brief コースの幅を取得
 * 
 * @param なし
 * 
 * @return コースの幅
 * 
 */
inline 	int Course::getWidth() const noexcept
{
	return m_width;
}

/**
 * @brief コースの高さを取得
 *
 * @param なし
 *
 * @return コースの高さ
 *
 */
inline int Course::getHeight() const noexcept
{
	return m_height;
}

inline int** Course::getParts() const noexcept
{
	return m_pParts;
}
