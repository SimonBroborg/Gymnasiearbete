#pragma once
#include <SDL\SDL.h>
#include <string>
#include "Sprite.h"
#include "Tile.h"
#include "Player.h"
#include "Circle.h"

#include "Engine.h"

class Manager : public Engine
{
public:

	

	


	

	//the different tile sprites
	const int TILE_RED = 0;
	const int TILE_GREEN = 1;
	const int TILE_BLUE = 2;

	//Initializing variables
	Manager();

	//Calls for close() upon game exit
	~Manager();

	//Runs the game 
	void run();

	std::vector<SDL_Rect> rects;

	float getHeight();
	float getWidth();

	bool loadMedia(Tile* tiles[], SDL_Renderer* renderer);

	void close(Tile* tiles[]);


	bool setTiles(Tile* tiles[]);

	//Start the game loop
	void gameLoop();

	

	bool bIsRunning;

	SDL_Event evnt;




	//scene textures
	Sprite tileTexture;
	
	
private:
	float m_frameTime = 0;
	float m_prevTime = 0;
	float m_currentTime = 0;
	float m_deltaTime = 0;

	SDL_Texture * backgroundTexture;
	
	SDL_Texture *loadBackground(std::string path);
};

