#include "Course.h"
#include "Part.h"
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
	m_quads.clear();

	// ボスステージの橋を更新
	m_bridgeController.update();

	// 叩いたときのブロックを追加する
	QUAD hitBlock;
	if (m_hitBlockController.update(hitBlock)) {
		m_quads.push_back(hitBlock);
	}

	// 描画するパーツを更新
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			int part = m_pParts[y][x];

			if (part == PART_NONE)
				continue;

			int textureIndex = part;

			switch (part) {
			case PART_COIN_0:
				m_coins.push_back(ivec2(x, y));
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
	glDrawArrays(GL_QUADS, 0, GLsizei(m_quads.size() * 4));
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
