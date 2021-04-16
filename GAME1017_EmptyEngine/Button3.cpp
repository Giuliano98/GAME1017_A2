#include "Button3.h"
#include "CollisionManager.h"
#include "Engine.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "StateManager.h"

Button3::Button3(SDL_Rect s, SDL_FRect d, const char* k) :SpriteObject(s, d), m_state(STATE_UP), m_key(k) {}

void Button3::Update()
{
	bool col = COMA::PointAABBCheck(EVMA::GetMousePos(), m_dst);
	switch (m_state)
	{
	case STATE_UP:
		if (col) m_state = STATE_OVER;
		break;
	case STATE_OVER:
		if (!col) m_state = STATE_UP;
		else if (col && EVMA::MousePressed(1)) m_state = STATE_DOWN; // 1 is left mouse.
		break;
	case STATE_DOWN:
		if (EVMA::MouseReleased(1)) // Left mouse released.
		{
			if (col)
			{
				m_state = STATE_OVER;
				Execute();	
			}
			else m_state = STATE_UP;
		}
		break;
	}
}

void Button3::Render()
{
	m_src.x = m_src.w * (int)m_state; 
	SDL_RenderCopyF(Engine::Instance().GetRenderer(), TEMA::GetTexture(m_key), &m_src, &m_dst);
}


// PlayButton class
PlayButton::PlayButton(SDL_Rect s, SDL_FRect d, const char* k) :Button3(s, d, k) {}

void PlayButton::Execute()
{
	STMA::ChangeState(new GameState());
}


// TitleButton class
TitleButton::TitleButton(SDL_Rect s, SDL_FRect d, const char* k) :Button3(s, d, k) {}

void TitleButton::Execute()
{
	STMA::ChangeState(new TitleState());
}


// ResumeButton class
ResumeButton::ResumeButton(SDL_Rect s, SDL_FRect d, const char* k) :Button3(s, d, k) {}

void ResumeButton::Execute()
{
	std::cout << "you click it in resume!!!!!" << std::endl;
	m_isPress = true;
}

bool ResumeButton::isExecute()
{
	return m_isPress;
}
