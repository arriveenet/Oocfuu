#pragma once

template<class entity_state>
class State {
public:
	virtual ~State() {};

	virtual void enter(entity_state*) = 0;

	virtual void execute(entity_state*) = 0;

	virtual void exit(entity_state*) = 0;
};

