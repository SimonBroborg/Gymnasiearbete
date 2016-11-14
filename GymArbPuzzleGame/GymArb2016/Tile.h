#pragma once
#include <SDL\SDL.h>
#include <string>
#include <vector>
#include "Engine.h"
#include "Sprite.h"

class Tile : public Engine
{
public:

	//initializes position and type
	Tile(int x, int y, int tileType);
	
	~Tile();

	//shows the tile
	void render(SDL_Rect& camera, Sprite &tileTexture, SDL_Renderer* gameRenderer, SDL_Rect tileClips[TOTAL_TILE_SPRITES]);

	//get the tile type
	int getType();

	//get the collision box
	SDL_Rect getBox();

private:
	//the attributes of the tile
	SDL_Rect m_box;

	//the tile type
	int m_type;
};