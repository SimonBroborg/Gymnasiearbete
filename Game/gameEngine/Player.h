#pragma once
#include <string>
#include <SDL\SDL.h>
#include <SDL\SDL_image.h>
#include "Manager.h"

class Player
{
public:
	Player();
	~Player();
	std::string playerImage;

	Manager game;

	const int HEIGHT = 50;
	const int WIDTH = 50;
	static const int maxSpeed = 5;

	float posX;
	float posY;
	float velX;
	float velY;
	

	void renderPlayer(SDL_Texture* tex, SDL_Renderer* ren, int posX, int posY);

	void move(SDL_Rect wall);

	void handleEvents(SDL_Event evnt);

	bool checkCollision(SDL_Rect a, SDL_Rect b);



private:
	SDL_Rect playerCollider;
};

