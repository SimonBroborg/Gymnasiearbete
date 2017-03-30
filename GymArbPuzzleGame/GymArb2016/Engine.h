#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <string>

class Engine
{
public:
	//Width and height of the game window
	const float SCREEN_WIDTH = 1260;
	const float SCREEN_HEIGHT = 720;


	//the dimensions of the level
	const int LEVEL_WIDTH = 1260;
	const int LEVEL_HEIGHT = 720;

	

	bool bIsRunning;

	//The game renderer
	SDL_Renderer *renderer;

	

	//Initializing the systems
	void initSystems();

	//The game window
	SDL_Window *window;

	double distanceSquared(int x1, int y1, int x2, int y2);
	
	Engine();
	~Engine();

	void Print(std::string text); 

	void keepInsideBorder(SDL_Rect posRect);

	
	
};

