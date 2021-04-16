#pragma once
#ifndef __MAIN_PLAYER__
#define __MAIN_PLAYER__
#include "GameObject.h"
#include "SDL.h"

#define GRAV 0.3
#define JUMPFORCE 9.0
#define  MAXVEL 4.0
#define  ACCEL 0.6


enum MainPlayerState { IDLING, RUNNING, JUMPING, ROLLING };

class MainPlayer : public AnimatedSpriteObject
{
public: // Methods.
	MainPlayer(SDL_Rect s, SDL_FRect d);
	void Update();
	void Render();

	// Check the player state
	void checkPlayerState();
	// update the player movement
	void playerMovement();
	// player and ground Collition check
	void checkPlayerAndObjColl(SDL_Rect g);
	
	// Movement tools
	void Stop();
	void StopX();
	void StopY();
	
	// Setters and Getters
	void SetAccelX(double a);
	void SetAccelY(double a);
	bool IsGrounded() const;
	void SetGrounded(bool g);
	double GetVelX() const;
	double GetVelY() const;
	void SetX(float x);
	void SetY(float y);
	const SDL_Rect GetPlayerPos();

private: // Properties.
	bool m_grounded;
	bool m_isPlayerDead;
	double m_accelX,
		m_accelY,
		m_velX,
		m_maxVelX,
		m_velY,
		m_maxVelY,
		m_drag,
		m_grav;
	MainPlayerState m_state;
};

#endif
