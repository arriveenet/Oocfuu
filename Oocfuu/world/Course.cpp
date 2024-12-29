#include "Course.h"
#include "Part.h"
#include "CourseManager.h"
#include "sound/Bgm.h"

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
	, m_courseType(Overworld)
	, m_clearColor(0L)
	, m_texture(TEXTURE_PARTS_OVERWORLD)
	, m_startPosition(0, 0)
	, m_nextWorld()
	, m_isLoaded(false)
	, m_quadsDirty(true)
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
	m_courseObjects.clear();
	m_courseObjects.shrink_to_fit();
}

/**
 * @brief コースの矩形を作成する
 *
 * @param なし
 *
 */
void Course::create()
{
	updateTotalQuads();
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

	for (Sprite* entitiy : m_courseObjects) {
		delete entitiy;
		entitiy = nullptr;
	}
	m_courseObjects.clear();
	m_courseObjects.shrink_to_fit();
}

/**
 * @brief コースの更新を行う
 *
 * @param なし
 *
 */
void Course::update()
{
	if (!m_isLoaded)
		return;

	m_coins.clear();
	//m_quads.clear();

	// 叩いたときのブロックを追加する
	QUAD hitBlock;
	if (m_hitBlockController.update(hitBlock)) {
		m_quads.push_back(hitBlock);
	}

	// ボスステージの橋を更新
	m_bridgeController.update();


	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			int part = m_pParts[y][x];

			if (part == PART_NONE)
				continue;

			int textureIndex = part;

			switch (part) {
			case PART_COIN_0:
			{
				m_coins.push_back(ivec2(x, y));
				int animationTable[] = { 0,1,2,2,1,0 };
				int animationTableLength = sizeof(animationTable) / sizeof(int);
				textureIndex += animationTable[(Game::m_count / 8) % animationTableLength];
			}
			break;
			case PART_QUESTION0:
			case PART_AXE_0:
			{
				int animationTable[] = { 0,1,2,2,1,0 };
				int animationTableLength = sizeof(animationTable) / sizeof(int);
				textureIndex += animationTable[(Game::m_count / 8) % animationTableLength];
			}
			break;
			case PART_SEA_0:
			{
				int animationTable[] = { 0,1,2,3,4,5,6,7 };
				int animationTableLength = sizeof(animationTable) / sizeof(int);
				textureIndex += animationTable[(Game::m_count / 16) % animationTableLength];
			}
			break;
			case PART_DESERT_1:
			{
				int animationTable[] = { 0,1,2,3,4,5,6,7 };
				int animationTableLength = sizeof(animationTable) / sizeof(int);
				textureIndex += animationTable[(Game::m_count / 16) % animationTableLength];

			}
			break;
			}
		}
	}

	updateTotalQuads();

	updateParts();
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

	g_textureManager.setTexture(m_texture);
	glDrawElements(GL_TRIANGLES, GLsizei(m_indices.size()), GL_UNSIGNED_SHORT, m_indices.data());
	g_textureManager.unbindTexture();

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_CULL_FACE);

	for (Sprite* entity : m_courseObjects) {
		entity->draw();
	}
}

/**
 * @brief コースの種別を設定する
 * 
 * @param[in] _typeName	コース種別の文字列
 * 
 * @return なし
 * 
 */
void Course::setType(const std::string _typeName)
{
	// 地上
	if (_typeName == "overworld") {
		m_courseType = Overworld;
		m_texture = TEXTURE_PARTS_OVERWORLD;
		Bgm::setBgm(Bgm::Ground);
	}
	// 地下
	else if (_typeName == "underground") {
		m_courseType = Underground;
		m_texture = TEXTURE_PARTS_UNDERGROUND;
		Bgm::setBgm(Bgm::Underground);
	}
	// 城
	else if (_typeName == "castle") {
		m_courseType = Castle;
		m_texture = TEXTURE_PARTS_CASTLE;
		Bgm::setBgm(Bgm::Castle);
	}
	// 上記以外
	else {
		assert(false);
	}
}

void Course::setPart(int x, int y, int part)
{
	if ((x < 0) || (x >= m_width)
		|| (y < 0) || (y >= m_height)) {
		return;
	}

	m_pParts[y][x] = part;
	m_quadsDirty = true;
}

void Course::updateTotalQuads()
{
	if (m_quadsDirty) {
		m_partToQuadIndex.clear();
		m_quads.resize(size_t(m_width * m_height));
		m_indices.resize(6 * size_t(m_width * m_height));
		m_partToQuadIndex.resize(size_t(m_width * m_height));

		int quadIndex = 0;
		for (int y = 0; y < m_height; y++) {
			for (int x = 0; x < m_width; x++) {
				int part = m_pParts[y][x];
				int partIndex = getPartIndexByPos(x, y);

				if (part == PART_NONE)
					continue;

				m_partToQuadIndex[partIndex] = quadIndex;

				auto& quad = m_quads[quadIndex];

				float left, right, top, bottom;

				left = static_cast<float>(x * PART_SIZE);
				right = static_cast<float>(left + PART_SIZE);
				top = static_cast<float>(y * PART_SIZE);
				bottom = static_cast<float>(top + PART_SIZE);

				quad.vertices[0].position = { left,  top    };
				quad.vertices[1].position = { left,  bottom };
				quad.vertices[2].position = { right, bottom };
				quad.vertices[3].position = { right, top    };

				const glm::vec2 textureSize = g_partManager.getTextureSize();

				Rect tileTexture = g_partManager.getRect(part);

				left = tileTexture.m_position.x / textureSize.x;
				right = left + (tileTexture.m_size.x / textureSize.x);
				top = tileTexture.m_position.y / textureSize.y;
				bottom = top + (tileTexture.m_size.y / textureSize.y);

				float ptx = static_cast<float>(1.0 / (textureSize.x * PART_SIZE));
				float pty = static_cast<float>(1.0 / (textureSize.y * PART_SIZE));

				quad.vertices[0].texCoord = { left  + ptx, top    + pty };
				quad.vertices[1].texCoord = { left  + ptx, bottom - pty };
				quad.vertices[2].texCoord = { right - ptx, bottom - pty };
				quad.vertices[3].texCoord = { right - ptx, top    + pty };

				quadIndex++;
			}
		}
	}

	m_quadsDirty = false;
}

void Course::updateParts()
{
	const float scroll = g_courseManager.getScroll();

	int yBegin = 0;
	int yEnd = SCREEN_HEIGHT / PART_SIZE;
	int xBegin = static_cast<int>(scroll / PART_SIZE);
	int xEnd = static_cast<int>(ceil((scroll + SCREEN_WIDTH) / PART_SIZE));

	for (int y = yBegin; y < yEnd; y++) {
		for (int x = xBegin; x < xEnd; x++) {
			int partIndex = getPartIndexByPos(x, y);
			if (m_pParts[y][x] == PART_NONE)
				continue;

			auto quadIndex = m_partToQuadIndex[partIndex];

			m_indices[(size_t)quadIndex * 6 + 0] = quadIndex * 4 + 0;
			m_indices[(size_t)quadIndex * 6 + 1] = quadIndex * 4 + 1;
			m_indices[(size_t)quadIndex * 6 + 2] = quadIndex * 4 + 2;
			m_indices[(size_t)quadIndex * 6 + 3] = quadIndex * 4 + 3;
			m_indices[(size_t)quadIndex * 6 + 4] = quadIndex * 4 + 0;
			m_indices[(size_t)quadIndex * 6 + 5] = quadIndex * 4 + 2;
		}
	}
}

