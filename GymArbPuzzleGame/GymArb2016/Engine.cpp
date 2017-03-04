#include "Engine.h"
#include <iostream>

Engine::Engine()
{

}


Engine::~Engine()
{
}




void Engine::initSystems()
{
	//Initializing SDL
	SDL_Init(SDL_INIT_VIDEO);
	if (SDL_Init == nullptr)
		std::cout << "Error: " << SDL_GetError() << std::endl;

	//Initializing SDL_Image
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(IMG_INIT_PNG) && imgFlags))
		std::cout << "Error: " << IMG_GetError();

	//Initializing SDL_ttf (true type font)
	if (TTF_Init() < 0)
		std::cout << "Error: " << TTF_GetError() << std::endl;

	//Initializing SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		std::cout << "Error: " << Mix_GetError() << std::endl;

	//Creates the game window
	window = SDL_CreateWindow("Puzzle game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
		std::cout << "Error: " << SDL_GetError() << std::endl;

	SDL_Surface* windowIcon = IMG_Load("assets/player/per.png");
	SDL_SetWindowIcon(window, windowIcon);
	SDL_FreeSurface(windowIcon); 

	//Creates the game renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
		std::cout << "Error: " << SDL_GetError() << std::endl;

}



double Engine::distanceSquared(int x1, int y1, int x2, int y2)
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX*deltaX + deltaY*deltaY;
}

//Makes sure the player cant go outside the window
void Engine::keepInsideBorder(SDL_Rect posRect)
{
	//makes sure the player cant go outside the window on the left
	if (posRect.x <= 0)
		posRect.x = 0;

	//makes sure the player cant go outside the window on the right
	if (posRect.x + posRect.w >= LEVEL_WIDTH)
		posRect.x = LEVEL_WIDTH - posRect.w;

	//the player cant fall through the bottom of the window
	if (posRect.y + posRect.h >= LEVEL_HEIGHT)
	{
		posRect.y = LEVEL_HEIGHT - posRect.h;
	}

	//the player cant move outside the top of the window
	if (posRect.y < 0)
		posRect.y = 0;
}



void Engine::Print(std::string text) {
	std::cout << text << std::endl;
}