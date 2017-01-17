#pragma once
////////////////////////////////////// ALL INCLUDED FILES //////////////////////////////////////////
#include <SDL\SDL.h>
#include <string>
#include <vector>
#include "Engine.h"
#include "Sprite.h"

///////////////////////////////////// THE Tile CLASS, INHERITS FROM Engine //////////////////////////
class Tile : public Engine
{
	//////////////////////////////////////////// PUBLIC ////////////////////////////////////////////////////////////////////
public:
	//Constructor
	Tile(int x, int y, int tileType);

	~Tile();

	//shows the tile
	void render(Sprite &tileTexture, SDL_Renderer* gameRenderer, SDL_Rect tileClips[TOTAL_TILE_SPRITES]);

	//get the tile type
	int getType();

	//get the collision box
	SDL_Rect getBox();

	void movePlatform(Tile* tiles[]);

	float getSpeed();

	void destroy(float delta);

	void rotate();

private:
	//the attributes of the tile
	SDL_Rect m_box;

	//the tile type
	int m_type;

	float m_damage;

	float m_velX;
	float m_velY;

	double angle;
};