#pragma once
#include "common/State.h"
#include "Kuribo.h"

class Kuribo;

/**
* Kuribo run state class.
*/
class KuriboStateRun : public State<Kuribo> {
private:
	KuriboStateRun() {};
	KuriboStateRun(const KuriboStateRun&) {};
	KuriboStateRun operator=(const KuriboStateRun&) { return KuriboStateRun(); };

public:
	static KuriboStateRun* instance();

	virtual void enter(Kuribo* _pKuribo);
	virtual void execute(Kuribo* _pKuribo);
	virtual void exit(Kuribo* _pKuribo);
};

/**
* Kuribo squish state class.
*/
class KuriboStateSquish : public State<Kuribo> {
private:
	KuriboStateSquish() {};
	KuriboStateSquish(const KuriboStateSquish&) {};
	KuriboStateSquish operator=(const KuriboStateSquish&) { return KuriboStateSquish(); };

public:
	static KuriboStateSquish* instance();

	virtual void enter(Kuribo* _pKuribo);
	virtual void execute(Kuribo* _pKuribo);
	virtual void exit(Kuribo* _pKuribo);
};

/**
* Kuribo die state class.
*/
class KuriboStateDie : public State<Kuribo> {
private:
	KuriboStateDie() {};
	KuriboStateDie(const KuriboStateDie&) {};
	KuriboStateDie operator=(const KuriboStateDie&) { return KuriboStateDie(); };

public:
	static KuriboStateDie* instance();

	virtual void enter(Kuribo* _pKuribo);
	virtual void execute(Kuribo* _pKuribo);
	virtual void exit(Kuribo* _pKuribo);
};
