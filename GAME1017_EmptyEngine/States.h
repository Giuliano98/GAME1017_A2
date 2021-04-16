#pragma once
#ifndef _STATES_H_
#define _STATES_H_

#include "Background.h"
#include "EmptyBox.h"
#include "TiledLevel.h"
#include "TitleSprite.h"

class State // This is the abstract base class for all states
{
public:
	virtual void Enter() = 0; // = 0 means pure virtual - must be defined in subclass
	virtual void Update() = 0; 
	virtual void Render();
	virtual void Exit() = 0;
	virtual void Resume(); 
	virtual ~State() {} // or = default;
protected: // Private but inherited
	State() {} // What does this prevent?
	map<std::string, GameObject*> m_objects;
	vector<EmptyBox*> m_EBvec;
	vector<Background*> m_BGvec;
	SDL_Rect m_groundCol;
	
	int m_cGap = 0,
	m_maxGap = 4;
	
};

class TitleState : public State
{
	TitleSprite m_titleSprite;
public:
	TitleState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};

class PauseState : public State
{
public:
	PauseState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};

class LoseState : public State
{
public:
	LoseState();
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

#endif

