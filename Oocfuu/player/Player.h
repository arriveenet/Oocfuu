#pragma once
#include "Sprite.h"
#include "PlayerStateContext.h"
#include "PlayerState.h"
#include "common/Message.h"
#include "common/Lyrics.h"

#include <vector>
#include <glm/glm.hpp>

#define PLAYER_SIZE			16

#define PLAYER_START_LEFT	3

#define PLAYER_KEY_DOWN		0x73
#define PLAYER_KEY_LEFT		0x61
#define PLAYER_KEY_RIGHT	0x64
#define PLAYER_KEY_JUMP		0x20

#define PLAYER_SPEED_MAX	2.5f
#define PLAYER_GRAVITY		0.4f

/**
 * @enum プレイヤーフラグ
*/
enum PLAYER_FLAG {
	PLAYER_FLAG_NONE = 0,
	PLAYER_FLAG_DEAD = 1,
	PLAYER_FLAG_GOAL = 2,
	PLAYER_FLAG_CLEAR = 4,
	PLAYER_FLAG_VISIBLE = 8,
	PLAYER_FLAG_JUMPING = 16,
	PLAYER_FLAG_FALLING = 32,
	PLAYER_FLAG_MAX = 16777215
};

class Player;
class PlayerStateContext;

/**
 * プレイヤークラス
*/
class Player : public Sprite{
	PlayerStateContext* m_pStateContext;

public:
	glm::vec2 m_speed;
	float m_acceleration;
	bool m_jumping;
	bool m_falling;
	std::vector<glm::vec2>m_leftPoints;
	std::vector<glm::vec2>m_rightPoints;
	std::vector<glm::vec2>m_bottomPoints;
	std::vector<glm::vec2>m_topPoints;
	char m_left;
	bool m_dead;
	bool m_goal;
	bool m_clear;
	bool m_visible;
	MessageController m_messageController;
	LyricsController m_lyricsController;

	Player();
	~Player();

	void init();
	void reset();
	void respawn(float _x, float _y);
	void update() override;
	void draw() override;
	void kill();
	void jump();
	void oneUp();
};

extern Player g_player;