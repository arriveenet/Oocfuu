#pragma once
#include "State.h"

template<class entity_type>
class StateMachine {
private:
	entity_type* m_pOwner;
	State<entity_type>* m_pCurrentState;
	State<entity_type>* m_pPreviousState;

public:
	StateMachine(entity_type* _owner)
		: m_pOwner(_owner)
		, m_pCurrentState(nullptr)
		, m_pPreviousState(nullptr)
	{
	}

	void setCurrentState(State<entity_type>* _state)
	{
		m_pCurrentState = _state;
	}

	void setPreviousState(State<entity_type>* _state)
	{
		m_pPreviousState = _state;
	}

	void update() const
	{
		if (m_pCurrentState)
			m_pCurrentState->execute(m_pOwner);
	}

	void changeState(State<entity_type>* _pNewState)
	{
		m_pPreviousState = m_pCurrentState;

		m_pCurrentState->exit(m_pOwner);

		m_pCurrentState = _pNewState;

		m_pCurrentState->enter(m_pOwner);
	}
};
