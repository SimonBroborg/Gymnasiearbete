#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

class Engine
{
public:
	//Width and height of the game window
	const int SCREEN_HEIGHT = 480;
	const int SCREEN_WIDTH = 640;

	//the dimensions of the level
	const int LEVEL_WIDTH = 640;
	const int LEVEL_HEIGHT = 480;

	static const int TOTAL_TILES = 48;
	static const int TOTAL_TILE_SPRITES = 3;

	//tile constants
	const int TILE_WIDTH = 80;
	const int TILE_HEIGHT = 80;

	//The game renderer
	SDL_Renderer *renderer;

	bool checkCollision(SDL_Rect a, SDL_Rect b);

	//Initializing the systems
	void initSystems();


	//The game window
	SDL_Window *window;


	SDL_Rect tileClips[TOTAL_TILE_SPRITES];
	Engine();
	~Engine();
};

