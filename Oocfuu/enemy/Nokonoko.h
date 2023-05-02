#pragma once
#include "Sprite.h"
#include "Collision.h"
#include "NokonokoStates.h"
#include "common/StateMachine.h"

#include <glm/glm.hpp>
#include <vector>

#define NOKONOKO_RUN_SPEED		0.5f
#define NOKONOKO_BOTTOM_COUNT	2

constexpr glm::vec2 NOKONOKO_RUN_SIZE{ 16.0f, 24.0f };
constexpr glm::vec2 NOKONOKO_SHELL_SIZE{ 16.0f, 16.0f };

enum NOKONOKO_STATE {
	NOKONOKO_STATE_RUN,
	NOKONOKO_STATE_SHELL,
	NOKONOKO_STATE_SPIN,
	NOKONOKO_STATE_RETURN,
	NOKONOKO_STATE_DEAD,
	NOKONOKO_STATE_MAX
};

class Nokonoko : public Sprite{
private:
	friend class NokonokoStateRun;
	friend class NokonokoStateShell;
	friend class NokonokoStateSpin;
	friend class NokonokoStateReturn;

public:
	// コンストラクタ
	Nokonoko();
	explicit Nokonoko(glm::vec2 _position);
	explicit Nokonoko(float _x, float _y);

	// コピーコンストラクタ
	Nokonoko(const Nokonoko& _nokonoko);

	// デストラクタ
	virtual ~Nokonoko();

	void update();
	void draw();
	void trun() { m_speed.x = -m_speed.x; }
	NOKONOKO_STATE getState() const { return m_state; }

protected:
	bool m_dead;
	bool m_falling;
	NOKONOKO_STATE m_state;
	int m_counter;
	glm::vec2 m_speed;
	glm::vec2 m_topPoint;
	glm::vec2 m_bottomPoints[NOKONOKO_BOTTOM_COUNT];
	glm::vec2 m_leftPoint;
	glm::vec2 m_rightPoint;
	StateMachine<Nokonoko>* m_pStateMachine;
};