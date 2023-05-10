#pragma once
#include "Enemy.h"
#include "KuriboStates.h"
#include "TextureManager.h"
#include"common/StateMachine.h"

#include <vector>
#include <glm/glm.hpp>

#define KURIBO_WIDTH			16
#define KURIBO_HEIGHT			16

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
	KURIBO_STATE getState()const { return m_state; }

protected:
	bool m_falling;
	KURIBO_STATE m_state;
	int m_counter;
	TEXTURE m_texture;
	glm::vec2 m_hitRightPoint;
	glm::vec2 m_hitLeftPoint;
	glm::vec2 m_topPoints[KURIBO_TOP_POINT_COUNT];
	StateMachine<Kuribo>* m_pStateMachine;
};