#pragma once
#include <SDL\SDL.h>
#include "Player.h"
#include "Circle.h"
#include <string>

class Manager
{
public:
	//Initializing variables
	Manager();

	//Calls for close() upon game exit
	~Manager();

	//Runs the game 
	void run();

	std::vector<SDL_Rect> rects;

	int getHeight();
	int getWidth();

private:

	//Start the game loop
	void gameLoop();

	//Initializing the systems
	void initSystems();

	void close();

	bool bIsRunning;

	SDL_Event evnt;

	//The game window
	SDL_Window *window;

	//The game renderer
	SDL_Renderer *renderer;

	//Width and height of the game window
	int m_windowHeight, m_windowWidth;

	float m_frameTime = 0;
	int m_prevTime = 0;
	int m_currentTime = 0;
	float m_deltaTime = 0;

	SDL_Texture * backgroundTexture;
	
	SDL_Texture *loadBackground(std::string path);	
};

