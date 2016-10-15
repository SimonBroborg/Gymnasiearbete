#pragma once
#include <SDL\SDL.h>
#include <SDL\SDL_image.h>
#include <iostream>
#include <string>
#include <SDL\SDL_ttf.h>
#include "Circle.h"
#include <vector>
#include <time.h>

class Manager
{
public:
	Manager();
	~Manager();

	const int SCREEN_HEIGHT = 480;
	const int SCREEN_WIDTH = 640;

	bool initSystems();
	bool loadGame();
	void run();

	SDL_Texture* playerOneTexture;

	SDL_Texture* loadTexture(std::string path);

	SDL_Window* globalWindow = NULL;
	SDL_Renderer* globalRenderer = NULL; 

private:

	bool bIsRunning;
};

