#pragma once
#include "common/State.h"
#include "Koopa.h"

// Idle state
class KoopaStateIdle : public State<Koopa>{
private:
	KoopaStateIdle() {};
	KoopaStateIdle(const KoopaStateIdle&) {};
	KoopaStateIdle operator=(const KoopaStateIdle&) { return KoopaStateIdle(); };

public:
	static KoopaStateIdle* instance();
	
	virtual void enter(Koopa* _pKoopa);
	virtual void execute(Koopa* _pKoopa);
	virtual void exit(Koopa* _pKoopa);
};

// Jump state
class KoopaStateJump : public State<Koopa> {
private:
	KoopaStateJump():m_lastFalling(false) {};
	KoopaStateJump(const KoopaStateJump&) :m_lastFalling(false) {};
	KoopaStateJump operator=(const KoopaStateJump&) { return KoopaStateJump(); };
	bool m_lastFalling;

public:
	static KoopaStateJump* instance();

	virtual void enter(Koopa* _pKoopa);
	virtual void execute(Koopa* _pKoopa);
	virtual void exit(Koopa* _pKoopa);
};

// Fire state
class KoopaStateFire : public State<Koopa> {
private:
	KoopaStateFire() {};
	KoopaStateFire(const KoopaStateFire&) {};
	KoopaStateFire operator=(const KoopaStateFire&) { return KoopaStateFire(); };

public:
	static KoopaStateFire* instance();

	virtual void enter(Koopa* _pKoopa);
	virtual void execute(Koopa* _pKoopa);
	virtual void exit(Koopa* _pKoopa);
};

// Die state
class KoopaStateDie : public State<Koopa> {
private:
	KoopaStateDie() : m_lastBridge(false){};
	KoopaStateDie(const KoopaStateDie&) : m_lastBridge(false) {};
	KoopaStateDie operator=(const KoopaStateDie&) { return KoopaStateDie(); };
	bool m_lastBridge;

public:
	static KoopaStateDie* instance();

	virtual void enter(Koopa* _pKoopa);
	virtual void execute(Koopa* _pKoopa);
	virtual void exit(Koopa* _pKoopa);
};
