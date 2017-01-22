#pragma once
#include <SDL\SDL.h>
#include "Tile.h"
#include "Engine.h"
#include "Sprite.h"
#include <string>
class WorldManager : public Engine
{
public:
	WorldManager();
	~WorldManager();



	//Keeps track of the current level
	int currentLevel;

	//The array which will contain the map tiles
	Tile* tileSet[TOTAL_TILES];

	//The array with with each tile clip from the sprite sheet
	SDL_Rect tileClips[TOTAL_TILE_SPRITES];

	//Loads the tiles to the level
	bool setTiles(Tile* tiles[], SDL_Rect tileClips[], std::string levelPath, int &playerX, int &playerY);

	//Renders the level
	void renderLevel(Sprite tileTexture, SDL_Rect tileClips[TOTAL_TILE_SPRITES], SDL_Renderer* renderer);

	void loadNextLevel(Tile* tiles[], SDL_Rect tileClips[TOTAL_TILE_SPRITES]);

	//Array which contains all the levels
	std::string levels[4]{ "assets/levels/level1.map", "assets/levels/level2.map" , "assets/levels/level3.map", "assets/levels/level4.map" };
	

};

