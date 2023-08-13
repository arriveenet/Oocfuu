#include "Course.h"
#include "TextureManager.h"
#include "Part.h"

#include <glm/glm.hpp>

using namespace glm;

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
	, m_nextWorld()
	, m_isLoaded(false)
{
}

/**
 * @brief デストラクタ
 *
 */
Course::~Course()
{
	m_pParts = nullptr;
	m_width = 0;
	m_height = 0;
	m_clearColor = 0;
	m_startPosition = { 0.0f, 0.0f };
	m_nextWorld = { 1, 1 };
	m_isLoaded = false;
	m_quads.clear();
	m_quads.shrink_to_fit();
}

/**
 * @brief コースの矩形を作成する
 *
 * @param なし
 *
 */
void Course::create()
{
	// scrolleColumn = (int)m_scroll / PART_SIZE;
	//printf("scrolleColumn=%d\n", scrolleColumn);
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			int part = m_pParts[y][x];
			if (part == PART_NONE)
				continue;

			int textureIndex = part;

			// パーツを追加する
			float x2 = (float)x * PART_SIZE;
			float y2 = (float)y * PART_SIZE;
			QUAD quad = {};
			const vec2 positions[4] =
			{
				{ x2, y2 },
				{ x2, y2 + PART_SIZE },
				{ x2 + PART_SIZE, y2 + PART_SIZE },
				{ x2 + PART_SIZE, y2 },
			};
			vec2* texCoords = g_partManager.getTexCoords(textureIndex);

			for (int i = 0; i < 4; i++) {
				quad.vertices[i].position = positions[i];
				quad.vertices[i].texCoord = texCoords[i];
			}
			m_quads.push_back(quad);
		}
	}
}

/**
 * @brief コースのパーツのメモリを解放し、初期化を行う
 *
 * @param なし
 *
 */
void Course::destroy()
{
	if (m_pParts) {
		for (int i = 0; i < m_height; ++i) {
			delete m_pParts[i];
		}
		delete[] m_pParts;
	}
	m_pParts = nullptr;

	m_width = 0;
	m_height = 0;
	m_clearColor = 0;
	m_startPosition = { 0.0f, 0.0f };
	m_nextWorld = { 1, 1 };
	m_isLoaded = false;
	m_quads.clear();
	m_quads.shrink_to_fit();
}

/**
 * @brief コースの更新を行う
 *
 * @param なし
 *
 */
void Course::update()
{

}

/**
 * @brief コースの描画を行う
 *
 * @param なし
 *
 */
void Course::draw()
{
	if (m_quads.empty() || !m_isLoaded)
		return;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(2, GL_FLOAT, sizeof(VERTEX), &m_quads[0].vertices[0].position);
	glTexCoordPointer(2, GL_FLOAT, sizeof(VERTEX), &m_quads[0].vertices[0].texCoord);

	g_textureManager.setTexture(TEXTURE_PARTS);
	glDrawArrays(GL_QUADS, 0, GLsizei(m_quads.size() * 4));
	g_textureManager.unbindTexture();

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_CULL_FACE);
}
