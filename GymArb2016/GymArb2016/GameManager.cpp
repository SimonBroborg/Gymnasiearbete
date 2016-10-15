#include "GameManager.h"
#include <iostream>
#include "Texture.h"


GameManager::GameManager()
{
	_window = nullptr;
	_renderer = nullptr;
	_screenWidth = 1366;
	_screenHeight = 768;

	_gameState = GameState::PLAY;


	
}


GameManager::~GameManager()
{
}

void GameManager::run()
{
	initSystems();
	gameLoop();
}

void GameManager::gameLoop()
{
	Texture texture;
	Texture player;
	while (_gameState != GameState::EXIT)
	{
		processInput();

		SDL_SetRenderDrawColor(_renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(_renderer);
		SDL_RenderPresent(_renderer);

	}
}

void GameManager::processInput()
{
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;

		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				_gameState = GameState::EXIT;
				break;
			}
			break;
		}
	}
}



bool GameManager::initSystems()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL_Init error: " << SDL_GetError();
		return 1;
	}
	else
	{
		std::cout << "SDL_Init successful!\n";
		_window = SDL_CreateWindow("Gymnasie spel", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_FULLSCREEN);
		if (_window == nullptr)
		{
			std::cout << "SDL_CreateWindow error: " << SDL_GetError();
			return 1;
		}
		else
		{
			std::cout << "SDL_CreateWindow successful!\n";
			_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (_renderer == nullptr)
			{
				std::cout << "SDL_CreateRendeer error: " << SDL_GetError();
				return 1;
			}
			else 
			{
				std::cout << "SDL_CreateRenderer successful!\n";
				if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
				{
					std::cout << "IMG_Init error: " << SDL_GetError();
					return 1;
				}
				else
				{
					std::cout << "IMG_Init successful!\n";
					if (TTF_Init() < 0)
					{
						std::cout << "TTF_Init could not be initialized!\n";
						return 1;
					}
					else
					{
						std::cout << "TTF_Init successful!\n";
					}
				}
			}
		}
	}
}


