#pragma once
#include <SDL\SDL.h>
#include <string>
#include "Sprite.h"
#include "Tile.h"
#include "Player.h"
#include "Circle.h"
#include <String>
#include "Menu.h"

#include "Engine.h"

class Manager : public Engine
{
public:
	

	//Initializing variables
	Manager();

	//Calls for close() upon game exit
	~Manager();

	//Runs the game 
	void run();

	std::vector<SDL_Rect> rects;

	float getHeight();
	float getWidth();

	bool loadMedia(Tile* tiles[], SDL_Renderer* renderer, SDL_Rect tileClips[TOTAL_TILE_SPRITES], std::string levelPath, int playerX, int playerY);

	void close(Tile* tiles[]);


	bool setTiles(Tile* tiles[], SDL_Rect tileClips[TOTAL_TILE_SPRITES], std::string levelPath, int playerX, int playerY);

	//Start the game loop
	void gameLoop();


	SDL_Event evnt;




	//scene textures
	Sprite tileTexture; 

	Sprite playerTexture;

	Sprite circleTexture;

	Sprite buttonTexture;

	void loadNextLevel(Tile* tiles[], SDL_Rect tileClips[TOTAL_TILE_SPRITES]);

	std::string levels[4]{ "assets/levels/level1.map", "assets/levels/level2.map" , "assets/levels/level3.map", "assets/levels/level4.map"};
	int currentLevel;
	
	
private:
	float m_frameTime = 0;
	float m_prevTime = 0;
	float m_currentTime = 0;
	float m_deltaTime = 0;

	SDL_Texture * backgroundTexture;
	
	SDL_Texture *loadBackground(std::string path);

	bool nextLevel;

	bool showMenu;
};

