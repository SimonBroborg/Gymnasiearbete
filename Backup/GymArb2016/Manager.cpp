#include "Manager.h"
#include <SDL\SDL_image.h>
#include <SDL\SDL_ttf.h>
#include <SDL\SDL_mixer.h>
#include <iostream>
#include "Player.h"
#include "Tile.h"
#include "Circle.h"

Manager::Manager()
{
	window = nullptr;
	renderer = nullptr;
	bIsRunning = true;
	m_windowHeight = 700;
	m_windowWidth = 1260;
}


Manager::~Manager()
{
}

void Manager::run()
{
	initSystems();
	gameLoop();
	close();
}

void Manager::gameLoop()
{
	Player player(renderer, 1, 1);
	Tile tile;


	//Map created by integers where 0 equals nothing
	int map[10][18] = {
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,1,2,2,3,2,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }

	};

	Mix_Music *bgm = Mix_LoadMUS("backgroundMusic.mp3");

	backgroundTexture = loadBackground("snowMountain.png");


	Circle circle(renderer, 70, 90);

	circle.loadSprite(renderer, "circle2.png", m_windowWidth / 2, m_windowHeight / 2);

	tile.createMap(renderer, map, rects); //creates the map based on a text file

	player.loadSprite(renderer, "gameCharacter.png", 0, m_windowHeight / 2);
	player.setUp();

	while (bIsRunning)
	{
		m_prevTime = m_currentTime;
		m_currentTime = SDL_GetTicks();
		m_deltaTime = (m_currentTime - m_prevTime) / 1000.0f;

		if (m_deltaTime < 0.03f)
			m_deltaTime = 0.03f;

		//plays the background music when the game starts
		if (!Mix_PlayingMusic()) {
			//Mix_PlayMusic(bgm, 0);
		}

		while (SDL_PollEvent(&evnt))
		{
			switch (evnt.type)
			{
			case SDL_QUIT:
				bIsRunning = false;
				break;

			case SDL_MOUSEBUTTONDOWN:
				circle.xPos = evnt.button.x;
				circle.yPos = evnt.button.y;
				player.posRect.x = evnt.button.x;
				player.posRect.y = evnt.button.x;
				break;

			case SDL_KEYDOWN:
				switch (evnt.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
					bIsRunning = false;
					break;
				case SDL_SCANCODE_A:
					circle.m_velX = -2;
					break;
				case SDL_SCANCODE_D:
					circle.m_velX = 2;
					break;

				case SDL_SCANCODE_SPACE:
					circle.m_velY = -6;
					break;
				}
				break;


			}
			player.processInput(evnt, m_deltaTime); //takes the players input as an SDL_Event

		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer); //clears the window

		//SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL); //copies the background to the renderer

		player.move(m_deltaTime, rects); //movement function for the player, calculates the new position of the player and checks collision

		circle.move(rects, player.posRect, player.getVelX() * m_deltaTime);

		//rects.clear(); //Clears the vector "rects" to avoid lag caused by to much memory use

		for (int i = 0; i < rects.size(); i++) {
			tile.render(rects[i]);
		}



		circle.draw();
		player.draw(); //renders the player
		SDL_RenderPresent(renderer); //prints out everything on the window
	}

}


SDL_Texture *Manager::loadBackground(std::string path)
{
	SDL_Texture *background = IMG_LoadTexture(renderer, path.c_str());

	return background;
}


void Manager::initSystems()
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
	window = SDL_CreateWindow("Puzzle game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight, SDL_WINDOW_SHOWN);
	if (window == nullptr)
		std::cout << "Error: " << SDL_GetError() << std::endl;

	//Creates the game renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
		std::cout << "Error: " << SDL_GetError() << std::endl;
}

void Manager::close()
{
	SDL_DestroyTexture(backgroundTexture);
	//quits all initialized systems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
	Mix_Quit();
}

float Manager::getHeight()
{
	return m_windowHeight;
}

float Manager::getWidth()
{
	return m_windowWidth;
}