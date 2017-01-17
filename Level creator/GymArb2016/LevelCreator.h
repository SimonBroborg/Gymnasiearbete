#pragma once
#include "Engine.h"
#include "Tile.h"
#include <SDL\SDL.h>
#include <SDL\SDL_image.h>
class LevelCreator : public Engine
{
public:
	LevelCreator();
	~LevelCreator();

	int currentType; //Tile type selector
 
	//creates the array which will contain the map tiles
	Tile* tileSet[TOTAL_TILES];

	//array with clips from the tile sprite sheet
	SDL_Rect tileClips[TOTAL_TILE_SPRITES];

	bool setTiles(std::string levelPath); //Loads the tiles to the level

	void putTile(int mouseX, int mouseY); //Puts out a new tile

	void saveTiles(std::string levelPath); //saves the current level

	void renderLevel(Sprite tileTexture, SDL_Renderer* renderer);
};

