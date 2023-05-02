#pragma once
#include "Enemy.h"
#include "KuriboStates.h"
#include"common/StateMachine.h"

#include <glm/glm.hpp>
#include <vector>

#define KURIBO_WIDTH			16
#define KURIBO_HEIGHT			16

#define KURIBO_SPEED			0.5f
#define KUROBO_FALL_SPEED		0.3f

#define KURIBO_TOP_POINT_COUNT	3

enum KURIBO_STATE {
	KURIBO_STATE_MOVE,
	KURIBO_STATE_SQUISH,
	KURIBO_STATE_DEAD,
	KURIBO_STATE_MAX
};

class Kuribo : public Enemy{
private:
	friend class KuriboStateRun;
	friend class KuriboStateSquish;
	friend class KuriboStateDie;

public:
	Kuribo();
	explicit Kuribo(glm::vec2 _position);
	explicit Kuribo(float _x, float _y);
	explicit Kuribo(const Kuribo& _kuribo);
	virtual ~Kuribo();

	void update() override;
	void draw() override;
	void kill() override;
	void intersectAllKuribo(std::vector<Kuribo>& kuribos);
	Rect getRect() const;

protected:
	bool m_falling;
	KURIBO_STATE m_state;
	int m_counter;
	glm::vec2 m_rightPoint;
	glm::vec2 m_leftPoint;
	glm::vec2 m_topPoints[KURIBO_TOP_POINT_COUNT];
	std::vector<glm::vec2> m_bottomPoints;
	StateMachine<Kuribo>* m_pStateMachine;
};