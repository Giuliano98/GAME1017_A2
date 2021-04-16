#include "Engine.h"
#include "EmptyBox.h"

#include "TextureManager.h"
#define SCROLLSPD 3

ObstacleSprite::ObstacleSprite(const SDL_Rect p) : m_dst(p), m_hasPlatf(false)
{
	
	
	switch (rand() % 3)
	{
	case 0:
		//m_color = { 255, 0, 0, 255 };
		m_enemyKey = "skeleton";
		break;
	case 1:
		//m_color = { 0, 255, 0, 255 };
		m_enemyKey = "bat";
		break;
	case 2:
		m_hasPlatf = true;
		m_enemyKey = "fire";
		//m_color = { 10, 100, 120, 255 };
		break;
	}
	
}

void ObstacleSprite::Render()
{
	if (m_enemyKey == "bat")
	{
		m_dst.y = 256;
		SDL_RenderCopy(Engine::Instance().GetRenderer(), TEMA::GetTexture(m_enemyKey), &m_src, &m_dst);
	}
	
	SDL_RenderCopy(Engine::Instance().GetRenderer(), TEMA::GetTexture(m_enemyKey), &m_src, &m_dst);
	
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), m_color.r, m_color.g, m_color.b, m_color.a);
	//SDL_RenderFillRect(Engine::Instance().GetRenderer(), &m_dst);
	if (m_hasPlatf)
	{
		SDL_Rect m_newDst = { m_dst.x, 280, m_platfSrc.w, m_platfSrc.h };
		//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 100, 100, 100, m_color.a);
		//SDL_RenderFillRect(Engine::Instance().GetRenderer(), &m_newDst);
		SDL_RenderCopy(Engine::Instance().GetRenderer(), TEMA::GetTexture("platf"), &m_platfSrc, &m_newDst);
	}
}


EmptyBox::EmptyBox(const SDL_Rect p, bool hasSprite) : m_pos(p), m_pObstSprite(nullptr)
{
	m_hasSprite = hasSprite;
	if (m_hasSprite)
		m_pObstSprite = new ObstacleSprite(m_pos);
	
}

EmptyBox::~EmptyBox()
{
	if (m_pObstSprite != nullptr) // Optional but reccomended. Canadian programming.
	{
		delete m_pObstSprite;
		m_pObstSprite = nullptr; // Optional wrangle.
	}
}

void EmptyBox::Update()
{
	m_pos.x -= SCROLLSPD; // Scroll the box.
	if (m_pObstSprite != nullptr)
		m_pObstSprite->m_dst.x = m_pos.x; // Scrolling sprite with box.
}

void EmptyBox::Render()
{
	// Render sprite.
	if (m_pObstSprite != nullptr)
		m_pObstSprite->Render();
	// Render white border. Extra for solution and supplemental video.
	//SDL_Rect m_dst = { m_pos.x, 256, 64, 128 };
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 255, 255, 0);
	//SDL_RenderDrawRect(Engine::Instance().GetRenderer(), &m_dst);
}

bool EmptyBox::getHasSprite()
{
	return m_hasSprite;
}

bool EmptyBox::checkObstHasPlatf()
{
	return this->m_pObstSprite->m_hasPlatf;
}

const SDL_Rect EmptyBox::GetSpritePos()
{
	return this->m_pObstSprite->m_dst;
}

const SDL_Rect EmptyBox::GetPlatfSpritePos()
{
	SDL_Rect m_newDst = { m_pObstSprite->m_dst.x, 280, m_pObstSprite->m_platfSrc.w, m_pObstSprite->m_platfSrc.h };
	return m_newDst;
}
