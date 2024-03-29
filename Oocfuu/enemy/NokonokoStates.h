#pragma once
#include "common/State.h"
#include "Nokonoko.h"

class Nokonoko;

/**
* Nokonoko run state class.
*/
class NokonokoStateRun : public State<Nokonoko>{
private:
	NokonokoStateRun() {};
	NokonokoStateRun(const NokonokoStateRun&) {};
	NokonokoStateRun operator=(const NokonokoStateRun&){ return NokonokoStateRun(); };

public:
	static NokonokoStateRun* instance();

	virtual void enter(Nokonoko* _pNokonoko);
	virtual void execute(Nokonoko* _pNokonoko);
	virtual void exit(Nokonoko* _pNokonoko);
};


/**
* Nokonoko shell state class.
*/
class NokonokoStateShell : public State<Nokonoko> {
private:
	NokonokoStateShell() {};
	NokonokoStateShell(const NokonokoStateShell&) {};
	NokonokoStateShell operator=(const NokonokoStateShell&) { return NokonokoStateShell(); };

public:
	static NokonokoStateShell* instance();

	virtual void enter(Nokonoko* _pNokonoko);
	virtual void execute(Nokonoko* _pNokonoko);
	virtual void exit(Nokonoko* _pNokonoko);
};

/**
* Nokonoko spin state class.
*/
class NokonokoStateSpin : public State<Nokonoko> {
private:
	NokonokoStateSpin() {};
	NokonokoStateSpin(const NokonokoStateSpin&) {};
	NokonokoStateSpin operator=(const NokonokoStateSpin&) { return NokonokoStateSpin(); };

public:
	static NokonokoStateSpin* instance();

	virtual void enter(Nokonoko* _pNokonoko);
	virtual void execute(Nokonoko* _pNokonoko);
	virtual void exit(Nokonoko* _pNokonoko);
};

/**
* Nokonoko return state class.
*/
class NokonokoStateReturn : public State<Nokonoko> {
private:
	NokonokoStateReturn() {};
	NokonokoStateReturn(const NokonokoStateReturn&) {};
	NokonokoStateReturn operator=(const NokonokoStateReturn&) { return NokonokoStateReturn(); };

public:
	static NokonokoStateReturn* instance();

	virtual void enter(Nokonoko* _pNokonoko);
	virtual void execute(Nokonoko* _pNokonoko);
	virtual void exit(Nokonoko* _pNokonoko);
};

class NokonokoStateDie : public State<Nokonoko> {
private:
	NokonokoStateDie() {}
	NokonokoStateDie(const NokonokoStateDie&) {}
	NokonokoStateDie operator=(const NokonokoStateDie&) { return NokonokoStateDie(); }

public:
	static NokonokoStateDie* instance();

	virtual void enter(Nokonoko* _pNokonoko);
	virtual void execute(Nokonoko* _pNokonoko);
	virtual void exit(Nokonoko* _pNokonoko);
};
