#pragma once
#include <iostream>
#include <SDL\SDL.h>
#include <SDL\SDL_image.h>
#include <iostream>
#include <string>
#include <SDL\SDL_ttf.h>
#include <math.h>
#include "Manager.h"
#include <vector>
#include "Engine.h"
#include "Tile.h"

class Circle : public Engine
{
public:
	Circle() {

	}
	Circle(SDL_Renderer *renderer,float radi, float angle);
	~Circle();

	void render(Sprite &circleTexture, SDL_Renderer* renderer);

	void move(Tile * tiles[], Player player);

	
	bool playerCollision(Player player);

	float m_theta;
	float m_step;
	float m_radi;
	float m_gravity;

	float xPos, yPos;

	float t;

	double m_velX, m_velY;

	float m_angle;
	float m_speed;
	float m_scaleX, m_scaleY;

	SDL_Rect m_imgRect;

private:

};

