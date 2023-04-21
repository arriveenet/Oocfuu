#pragma once
#include "Sprite.h"
#include "KoopaFire.h"
#include "common/StateMachine.h"
#include "common/range.h"

#include <vector>
#include <glm/glm.hpp>

#define KOOPA_WIDTH				32
#define KOOPA_HEIGHT			32
#define KOOPA_FALL_SPEED		0.14f
#define KOOPA_JUMP_COUNT_MAX	8
#define KOOPA_UPDATE_DISTANCE	688.0f
#define KOOPA_FIRE_MAX			2

enum KOOPA_STATE {
	KOOPA_STATE_IDLE,
	KOOPA_STATE_JUMP,
	KOOPA_STATE_FIRE,
	KOOPA_STATE_DIE,
	KOOPA_STATE_MAX
};

enum KOOPA_MOVE {
	KOOPA_MOVE_FRONT,
	KOOPA_MOVE_BACK,
	KOOPA_MOVE_STOP,
	KOOPA_MOVE_MAX,
};

enum KOOPA_FLAG {
	KOOPA_FLAG_DEAD			= 0x01,
	KOOPA_FLAG_JUMPING		= 0x02,
	KOOPA_FLAG_FALLING		= 0x04,
	KOOPA_FLAG_COLLISION	= 0x08,
	KOOPA_FLAG_ANIMATION	= 0x10,
};

class Koopa : public Sprite {
private:
	friend class KoopaStateIdle;
	friend class KoopaStateJump;
	friend class KoopaStateFire;
	friend class KoopaStateDie;

public:
	Koopa();
	explicit Koopa(glm::vec2 _position);
	Koopa(float _x, float _y);
	virtual ~Koopa();

	void reset();
	void update() override;
	void draw() override;
	void fire();
	void kill();
	bool isDead();
	void setActionRange(const RANGE& _range);

protected:
	void nextMovement();

	int m_flag;
	int m_jumpCount;
	KOOPA_STATE m_state;
	int m_counter;
	StateMachine<Koopa>* m_pStateMachine;
	glm::vec2 m_speed;
	std::vector<glm::vec2> m_bottomPoints;
	KoopaFire m_fires[KOOPA_FIRE_MAX];
	RANGE m_actionRange;
};
