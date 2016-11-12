#pragma once
#include <SDL\SDL.h>
#include <string>
#include <vector>
#include "Sprite.h"

class Tile : public Sprite
{
public:
	const int X_TILES = 20;
	const int Y_TILES = 15;

	const int TILE_WIDTH = 70;
	const int TILE_HEIGHT = 70;
	const int TOTAL_TILES = 1;
	static const int TOTAL_TILE_SPRITES = 3;

	const int TILE_RED = 0;
	const int TILE_BLUE = 1;
	const int TILE_GREEN = 2;

	const int LEVEL_WIDTH = 1280;
	const int LEVEL_HEIGHT = 700;

	SDL_Rect tileClips[TOTAL_TILE_SPRITES];

	Tile();
	
	~Tile();

	void render(Sprite tileTexture);

	int getType();

	SDL_Rect getBox();

	bool setTiles(Tile* tiles[]);

	SDL_Rect rTileClips[3];

	SDL_Texture *loadTile(SDL_Renderer *renderer, std::string path);

	void createMap(SDL_Renderer *renderer, int map[10][18], std::vector<SDL_Rect> &rRects);

	void render(SDL_Rect tileRect);
private:
	SDL_Rect m_box;

	int m_type;

	SDL_Texture* tileSprite;
	SDL_Rect tileRect;



};