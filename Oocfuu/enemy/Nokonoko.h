#pragma once
#include "Enemy.h"
#include "NokonokoStates.h"
#include "common/StateMachine.h"

#include <glm/glm.hpp>
#include <vector>

constexpr glm::vec2 NOKONOKO_RUN_SIZE{ 16.0f, 24.0f };
constexpr glm::vec2 NOKONOKO_SHELL_SIZE{ 16.0f, 16.0f };
constexpr int NOKONOKO_TOP_POINT_COUNT = 3;

enum NOKONOKO_STATE {
	NOKONOKO_STATE_RUN,
	NOKONOKO_STATE_SHELL,
	NOKONOKO_STATE_SPIN,
	NOKONOKO_STATE_RETURN,
	NOKONOKO_STATE_DEAD,
	NOKONOKO_STATE_MAX
};

class Nokonoko : public Enemy {
private:
	friend class NokonokoStateRun;
	friend class NokonokoStateShell;
	friend class NokonokoStateSpin;
	friend class NokonokoStateReturn;
	friend class NokonokoStateDie;

public:
	enum class Type {
		Green,
		Red,
	};

	// コンストラクタ
	Nokonoko();
	explicit Nokonoko(glm::vec2 _position);
	explicit Nokonoko(float _x, float _y);

	// コピーコンストラクタ
	Nokonoko(const Nokonoko& _nokonoko);

	// デストラクタ
	virtual ~Nokonoko();

	void update() override;
	void draw() override;
	void kill() override;
	NOKONOKO_STATE getState() const { return m_state; }
	Type getType() const { return m_type; }
	void setType(Type type);

protected:
	NOKONOKO_STATE m_state;
	Nokonoko::Type m_type;
	int m_counter;
	glm::vec2 m_topPoints[NOKONOKO_TOP_POINT_COUNT];
	StateMachine<Nokonoko>* m_pStateMachine;
};