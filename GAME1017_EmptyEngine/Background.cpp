#include "Background.h"

#include "TextureManager.h"

Background::Background(int BGType, SDL_Rect dst) : m_dst(dst)
{
	TEMA::Load("Img/background1.png", "BG");
	TEMA::Load("Img/background2.png", "MG");
	TEMA::Load("Img/background3.png", "FG");

	m_src = {0,0,768,576};

	
	switch (BGType)
	{
	case BACKGROUND:
		BGKey = "BG";
		m_scrollSpeed = BGVEL;
		break;
	case MIDGROUND:
		BGKey = "MG";
		m_scrollSpeed = MGVEL;
		break;
	case FOREGROUND:
		BGKey = "FG";
		m_scrollSpeed = FGVEL;
		break;
	}
}

void Background::Update()
{
	if (m_dst.x <= -m_dst.w)
		m_dst.x = m_dst.w;
	
	m_dst.x -= m_scrollSpeed;
}

void Background::Render()
{
	SDL_RenderCopy(Engine::Instance().GetRenderer(), TEMA::GetTexture(BGKey), &m_src, &m_dst);
}
