#pragma once
#include <SDL\SDL.h>
#include "Tile.h"
#include "Engine.h"
#include "Sprite.h"
#include <string>
#include "Player.h"

class WorldManager : public Engine
{
public:
	WorldManager(SDL_Renderer* renderer);
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


	static const int TOTAL_LEVELS = 5;
	//Array which contains all the levels
	std::vector<std::string>levels;
	
	std::string backgroundPath; 
	//Loads the next level
	void loadNextLevel(Player & player);

	int playerStartX, playerStartY;

	std::string getBackground();

bool nextLevel; 
SDL_Texture* backgroundTexture;
private:

	SDL_Renderer* gameRenderer; 


};

