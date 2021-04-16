#pragma once
#ifndef __TITLE_SPRITE__
#define __TITLE_SPRITE__
#include "GameObject.h"

class TitleSprite : public GameObject
{
	SDL_Rect src, dst;
public:
	TitleSprite(SDL_FRect d) : GameObject(d)
	{
		TEMA::Load("Img/TitleSprite.png", "title");
		src = { (int)d.x, (int)d.y,(int)d.w,(int)d.h };
	}
	void Update(){}
	void Render() override
	{
		SDL_RenderCopyF(Engine::Instance().GetRenderer(), TEMA::GetTexture("title"), &src, &m_dst);
		//SDL_RenderCopy(Engine::Instance().GetRenderer(), TEMA::GetTexture("title"), &src, &dst);
	}
};

#endif