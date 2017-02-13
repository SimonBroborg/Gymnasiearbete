#pragma once
#include <SDL\SDL.h>
#include <string>
#include "Sprite.h"
#include "Tile.h"
#include "Player.h"
#include "Circle.h"
#include <String>
#include "Menu.h"
#include <SDL\SDL_messagebox.h>

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

	bool loadMedia(Tile* tiles[], SDL_Renderer* renderer, SDL_Rect tileClips[TOTAL_TILE_SPRITES], std::string levelPath, int &playerX, int &playerY);

	void close(Tile* tiles[]);

	void gameLoop();


	SDL_Event evnt;



	//scene textures
	Sprite tileTexture; 
	Sprite playerTexture;

	Sprite circleTexture;

	Sprite cursorTexture;	
	
private:
	float m_frameTime = 0;
	float m_prevTime = 0;
	float m_currentTime = 0;
	float m_deltaTime = 0;

	bool m_fullscreen;  //Flag for fullscreen 

	//SDL_Texture * backgroundTexture; //texture for the background
	
	SDL_Texture *loadBackground(std::string path); //Loads a new background texture


	bool showMenu;
};

