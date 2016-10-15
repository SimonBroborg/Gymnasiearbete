#pragma once
#include <SDL\SDL.h>
#include <SDL\SDL_image.h>
#include <SDL\SDL_ttf.h>
#include "Timer.h"
#include "Texture.h"

enum class GameState { PLAY, EXIT };
class GameManager
{
public:

	GameManager();
	~GameManager();

	Timer timer;
	std::stringstream timeText;
	

	void run();

private:

	bool initSystems();
	void gameLoop();
	void processInput();


	SDL_Window* _window;
	SDL_Renderer* _renderer;

	

	int _screenWidth;
	int _screenHeight;

	GameState _gameState;

};

