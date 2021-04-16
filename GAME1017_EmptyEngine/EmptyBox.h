#pragma once
#ifndef __EMPTY_BOX__
#define __EMPTY_BOX__
#include <SDL.h>

class ObstacleSprite // Represents visual component of Box.
{
private:
	SDL_Rect m_src = {0,0,64,64};
	SDL_Rect m_dst; // Position on screen.
	SDL_Color m_color; // Random color for box.
	std::string m_enemyKey;
	SDL_Rect m_platfSrc = { 0,0,64,24 };
	bool m_hasPlatf;
public:
	ObstacleSprite(const SDL_Rect p);
	void Render();
	friend class EmptyBox; // Or make a setter for m_dst.
};

class EmptyBox // Obstacle parent/proxy for the Sprite.
{
private:
	SDL_Rect m_pos; // Position on screen. Y is optional.
	ObstacleSprite* m_pObstSprite; // Pointer to visual component.
	bool m_hasSprite;
public:
	EmptyBox(const SDL_Rect p, bool hasSprite = false);
	~EmptyBox();
	void Update();
	void Render();
	const SDL_Rect GetPos() { return m_pos; }
	bool getHasSprite();
	bool checkObstHasPlatf();
	const SDL_Rect GetSpritePos();
	const SDL_Rect GetPlatfSpritePos();
};

#endif