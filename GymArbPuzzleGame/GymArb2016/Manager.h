#pragma once
#include <SDL\SDL.h>
#include <string>
#include "Sprite.h"
#include "Tile.h"
#include "Player.h"
#include "Circle.h"
#include <String>

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

	bool loadMedia(Tile* tiles[], SDL_Renderer* renderer, SDL_Rect tileClips[TOTAL_TILE_SPRITES]);

	void close(Tile* tiles[]);


	bool setTiles(Tile* tiles[], SDL_Rect tileClips[TOTAL_TILE_SPRITES], std::string mapPath);

	//Start the game loop
	void gameLoop();

	

	bool bIsRunning;

	SDL_Event evnt;




	//scene textures
	Sprite tileTexture; 

	Sprite playerTexture;
	
	
private:
	float m_frameTime = 0;
	float m_prevTime = 0;
	float m_currentTime = 0;
	float m_deltaTime = 0;

	SDL_Texture * backgroundTexture;
	
	SDL_Texture *loadBackground(std::string path);
};

