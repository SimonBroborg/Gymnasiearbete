#include "Engine.h"
#include <iostream>

Engine::Engine()
{

}


Engine::~Engine()
{
}


bool Engine::checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

bool Engine::checkCircularCollision(SDL_Rect player, SDL_Rect circle, float velX, float velY)
{
	//Closest point on collision box
	int cX, cY;

	//Find closest x offset
	if (player.x < circle.x)
	{
		cX = circle.x;
	}
	else if (player.x > circle.x + circle.w)
	{
		cX = circle.x + circle.w;
	}
	else
	{
		cX = player.x;
	}
	if (player.y < circle.y)
	{
		cY = circle.y;
	}
	else if (player.y > circle.y + circle.h)
	{
		cY = circle.y + circle.h;
	}
	else
	{
		cY = player.y;
	}

	//if the closest point is inside the circle
	if (((cX - circle.x) * (cX - circle.x) + (cY - circle.y) * (cY - circle.y)) < circle.w / 2 * circle.w / 2)
	{
		//This rect and the circle have collided
		return true;
	}

	

	//If the shapes have not collided
	return false;
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

bool Engine::circularCollision(SDL_Rect circle, SDL_Rect rect)
{

	//Closest point on collision box
	int cX, cY;

	//Find closest x offset
	if (circle.x + circle.w < rect.x)
	{
		cX = rect.x;
	}
	else if (circle.x + circle.w > rect.x + rect.w)
	{
		cX = rect.x + rect.w;
	}
	else
	{
		cX = circle.x + circle.w;
	}

	//Find closest y offset
	if (circle.y + circle.h < rect.y)
	{
		cY = rect.y;
	}
	else if (circle.y + circle.h > rect.y + rect.h)
	{
		cY = rect.y + rect.h;
	}
	else
	{

		cY = circle.y + circle.h;
	}
	//If the closest point is inside the circle
	if (distanceSquared(circle.x + circle.w, circle.y + circle.h, cX, cY) < circle.w * circle.w)
	{
		//This box and the circle have collided
		return true;
	}

	//If the shapes have not collided
	return false;

}