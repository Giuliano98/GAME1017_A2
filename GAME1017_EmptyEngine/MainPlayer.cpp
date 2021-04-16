#include "MainPlayer.h"


#include "CollisionManager.h"
#include "Engine.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "TextureManager.h"

MainPlayer::MainPlayer(SDL_Rect s, SDL_FRect d) :
AnimatedSpriteObject(s, d), m_state(JUMPING), m_grounded(false),
m_maxVelX(MAXVEL), m_maxVelY(JUMPFORCE), m_grav(GRAV), m_drag(0.86), m_isPlayerDead(false)
{
	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	
	
	// Initialize the jump animation.
	SetAnimation(10, 1, 3,0);
}

void MainPlayer::Update()
{
	// Checking states.
	checkPlayerState();
	// MainPlayer movement
	playerMovement();
	// Invoke the animation.
	Animate();
}

void MainPlayer::Render()
{
	// DRAW THE PLAYER
	const SDL_Rect m_myRect = { m_dst.x, m_dst.y,m_dst.w,m_dst.h, };
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 100, 225, 225);
	//SDL_RenderFillRect(Engine::Instance().GetRenderer(), &m_myRect);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), TEMA::GetTexture("mainPlayer"),
		&m_src, &m_myRect);
}

void MainPlayer::checkPlayerState()
{
	// set edge in x
	if (m_dst.x < 0)
		m_dst.x = 0;
	if (m_dst.x + m_dst.w > WIDTH)
		m_dst.x = WIDTH - m_dst.w;
	
	// check the current player's state
	switch (m_state)
	{
	case IDLING:
		// Transition to run.
		if (EVMA::KeyPressed(SDL_SCANCODE_A) || EVMA::KeyPressed(SDL_SCANCODE_D))
		{
			m_state = RUNNING;
			SetAnimation(8, 0, 5, 96); 
		}
		// Transition to jump.
		else if ((EVMA::KeyPressed(SDL_SCANCODE_SPACE) || EVMA::KeyPressed(SDL_SCANCODE_W)) && m_grounded)
		{
			m_accelY = -JUMPFORCE; // SetAccelY(-JUMPFORCE);
			m_grounded = false; // SetGrounded(false);
			m_state = JUMPING;
			SoundManager::Instance().playSound("jump", 1);
			SetAnimation(10, 0, 3, 0);
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_S) && m_grounded)
		{
			// rolling animation
			m_state = ROLLING;
			m_src.y += 24;
			m_src.h -= 24;
			m_dst.h = 64;
			SetAnimation(10, 0, 4, 224);
		}
		
		break;
	case RUNNING:
		// Move left and right.
		if (EVMA::KeyHeld(SDL_SCANCODE_A) && m_dst.x > 0)
			m_accelX = -ACCEL;
		else if (EVMA::KeyHeld(SDL_SCANCODE_D) && m_dst.x < WIDTH - m_dst.w)
			m_accelX = ACCEL;
		// Transition to jump.
		if( (EVMA::KeyPressed(SDL_SCANCODE_SPACE) || EVMA::KeyPressed(SDL_SCANCODE_W) ) && m_grounded )
		{
			m_accelY = -JUMPFORCE;
			m_grounded = false;
			m_state = JUMPING;
			SoundManager::Instance().playSound("jump", 1);
			SetAnimation(10, 0, 3, 0);
		}
		if (EVMA::KeyHeld(SDL_SCANCODE_S) && m_grounded)
		{
			// rolling animation
			m_state = ROLLING;
			m_src.y += 24;
			m_src.h -= 24;
			m_dst.h = 64;
			SetAnimation(10, 0, 4, 224);
		}
		// Transition to idle.
		if (!EVMA::KeyHeld(SDL_SCANCODE_A) && !EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			m_state = IDLING;
			SetAnimation(8, 0, 5, 96);
		}
		break;
	case JUMPING:
		// Move in mid-air is cool.
		if (EVMA::KeyHeld(SDL_SCANCODE_A) && m_dst.x > 0)
			m_accelX = -ACCEL;
		else if (EVMA::KeyHeld(SDL_SCANCODE_D) && m_dst.x < WIDTH - m_dst.w)
			m_accelX = ACCEL;
		// Hit the ground, transition to run.
		if (m_grounded)
		{
			m_state = RUNNING;
			SetAnimation(8, 0, 5, 96);
		}
		break;
	case ROLLING:
		// Move in mid-roll is cool.
		if (EVMA::KeyHeld(SDL_SCANCODE_A) && m_dst.x > 0)
			m_accelX = -ACCEL;
		else if (EVMA::KeyHeld(SDL_SCANCODE_D) && m_dst.x < WIDTH - m_dst.w)
			m_accelX = ACCEL;
		// animate rolling
		// Transition to jump.
		if ((EVMA::KeyPressed(SDL_SCANCODE_SPACE) || EVMA::KeyPressed(SDL_SCANCODE_W)) && m_grounded)
		{
			m_accelY = -JUMPFORCE;
			m_grounded = false;
			m_state = JUMPING;
			m_src.y -= 24;
			m_src.h += 24;
			m_dst.y -= 24;
			m_dst.h = 96;
			SoundManager::Instance().playSound("jump", 1);
			SetAnimation(10, 0, 3, 0);
		}
		if (!EVMA::KeyHeld(SDL_SCANCODE_S))
		{
			m_state = IDLING;
			m_src.y -= 24;
			m_src.h += 24;
			m_dst.y -= 24;
			m_dst.h = 96;
			SetAnimation(8, 0, 5, 96);
		}
		
		break;
	}
}

void MainPlayer::playerMovement()
{
	// MainPlayer movement. X axis first.
	m_velX += m_accelX;
	m_velX *= (m_grounded ? m_drag : 1.0); // Cheeky deceleration.
	m_velX = std::min(std::max(m_velX, -m_maxVelX), m_maxVelX);
	m_dst.x += (float)m_velX; // May have to cast to (int)

	m_velY += m_accelY + m_grav;
	m_velY = std::min(std::max(m_velY, -m_maxVelY), m_maxVelY); // (m_grav * 5.0)
	m_dst.y += (float)m_velY;

	m_accelX = m_accelY = 0.0; // Resetting accel every frame.
}

void MainPlayer::checkPlayerAndObjColl(SDL_Rect g)
{
	const SDL_Rect p = { (int)m_dst.x, (int)m_dst.y,(int)m_dst.w,(int)m_dst.h, };
	if (COMA::AABBCheck(p, g))
	{
		if ((p.y + p.h) - JUMPFORCE <= g.y)
		{ // Colliding with top side of the obstacle.
			StopY();
			SetY(g.y - p.h);
			SetGrounded(true);
		}
		else if (p.y + JUMPFORCE >= (g.y + g.h))
		{ // Colliding with bottom side of tile.
			
		}
		else if ((p.x + p.w) - JUMPFORCE <= g.x)
		{ // Colliding with left side of tile.
			StopX();
			SetX(g.x - p.w);
		}
		else if (p.x + JUMPFORCE >= (g.x + g.w))
		{ // Colliding with right side of tile.
			StopX();
			SetX(g.x + g.w);
		}
	}
}

void MainPlayer::Stop() { StopX(); StopY(); }

void MainPlayer::StopX() { m_velX = 0.0; }

void MainPlayer::StopY() { m_velY = 0.0; }

void MainPlayer::SetAccelX(double a) { m_accelX = a; }

void MainPlayer::SetAccelY(double a) { m_accelY = a; }

bool MainPlayer::IsGrounded() const { return m_grounded; }

void MainPlayer::SetGrounded(bool g) { m_grounded = g; }

double MainPlayer::GetVelX() const { return m_velX; }

double MainPlayer::GetVelY() const { return m_velY; }

void MainPlayer::SetX(float x) { m_dst.x = x; }

void MainPlayer::SetY(float y) { m_dst.y = y; }

const SDL_Rect MainPlayer::GetPlayerPos()
{
	return { (int)m_dst.x, (int)m_dst.y,(int)m_dst.w,(int)m_dst.h, };
}
