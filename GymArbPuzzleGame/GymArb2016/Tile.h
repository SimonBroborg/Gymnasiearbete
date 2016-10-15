#pragma once
#include <SDL\SDL.h>
#include <string>
#include <vector>

class Tile
{
public:
	Tile();
	~Tile();

	//Loads the tile texture from a file path
	SDL_Texture* loadTile(SDL_Renderer *renderer, std::string path);

	//The amount of vertical and horizontal tiles
	const int X_TILES = 20;
	const int Y_TILES = 15;

	//creates the map based on the tiles inside the vector rects
	void createMap(SDL_Renderer *renderer, int map[15][20], std::vector<SDL_Rect> &rRects);

	//tile texture
	SDL_Texture *tileSprite;

	SDL_Rect tileRect;

};

