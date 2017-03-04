#pragma once
#include <SDL\SDL.h>
#include <SDL\SDL_image.h>
#include <fstream>
#include <iostream>

#include <string>
#include "Player.h"
#include "Tile.h"

class Tile;
class Player; 

class WorldManager : public Engine
{
public:
	WorldManager(SDL_Renderer* renderer);

	~WorldManager();

	static const int TOTAL_TILES = 1008;
	static const int TOTAL_TILE_SPRITES = 21;

	//tile constants
	const int TILE_WIDTH = 30;
	const int TILE_HEIGHT = 30;

	//the different tile sprites
	const int TILE_NONE = 0;
	const int TILE_BRIDGE = 1;
	const int TILE_BOX = 2;
	const int TILE_GRASS_ROUNDED = 3;
	const int TILE_DIRT_CENTER = 4;
	const int TILE_GRASS_CLIFF_LEFT = 5;
	const int TILE_GRASS_CLIFF_RIGHT = 6;
	const int TILE_GRASS_LEFT_ROUNDED = 7;
	const int TILE_GRASS_CENTER = 8;
	const int TILE_GRASS_RIGHT_ROUNDED = 9;
	const int TILE_PORTAL = 10;
	const int TILE_MOVING_PLATFORM = 11;
	const int TILE_MOVING_PLATFORM_STOP = 12;
	const int TILE_SPIKES = 13;

	const int TILE_ICE_WHOLE = 14;
	const int TILE_ICE_BROKEN_1 = 15;
	const int TILE_ICE_BROKEN_2 = 16;
	const int TILE_ICE_BROKEN_3 = 17;

	const int TILE_SAW_1 = 18;
	const int TILE_SPRING = 19;

	const int TILE_PLAYER_SPAWN = 20;


	//Keeps track of the current level
	int currentLevel;
	
	//The array which will contain the map tiles
	Tile *tileSet[TOTAL_TILES];

	//The array with with each tile clip from the sprite sheet
	SDL_Rect tileClips[TOTAL_TILE_SPRITES];

	//Loads the tiles to the level
	bool setTiles(Tile* tiles[], SDL_Rect tileClips[], std::string levelPath, int &playerX, int &playerY);

	//Renders the level
	void renderLevel(Sprite tileTexture, SDL_Rect tileClips[TOTAL_TILE_SPRITES], SDL_Renderer* renderer);

	static const int TOTAL_LEVELS = 4;
	//Array which contains all the levels
	std::vector<std::string>levels;
	

	//Loads the next level
	void loadNextLevel(Player & player);

	int playerStartX, playerStartY;

	void setBackground(std::string path);

bool nextLevel; 

SDL_Texture* backgroundTexture;

private:

	SDL_Renderer* gameRenderer; 


};

