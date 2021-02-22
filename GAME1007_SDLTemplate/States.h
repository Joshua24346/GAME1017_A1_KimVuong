#pragma once
#ifndef _STATES_
#define _STATES_

class State // This is the abstract base class for all states
{
public:
	virtual void Enter() = 0; // = 0 means pure virtual - must be defined in subclass
	virtual void Update() = 0;
	virtual void Render();
	virtual void Exit() = 0;
	virtual void Resume();
	virtual ~State() = default; // Modern alternative to {}
protected: // Private but inherited
	State() {}; // What does this prevent?
};

class TitleState : public State
{
public:
	TitleState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};

class GameState : public State
{
public:
	GameState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
	virtual void Resume();
};

enum States {
	NONE = -1,
	START,
	PAUSE,
	PLAY,
	LOST,
	END
};

#endif

