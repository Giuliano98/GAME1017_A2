#pragma once
#ifndef __BACKGROUND__
#define __BACKGROUND__

#define BGVEL 1
#define MGVEL 2
#define FGVEL 3

#include "Engine.h"


enum backgroundType
{
	BACKGROUND,
	MIDGROUND,
	FOREGROUND
};


class Background
{
private:
	int m_scrollSpeed;
	SDL_Rect m_src, m_dst;
	std::string BGKey;
public:
	Background(int BGType, SDL_Rect dst);
	
	void Update();
	void Render();
};

#endif