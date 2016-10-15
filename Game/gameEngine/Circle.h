#pragma once
#include <iostream>
#include <SDL\SDL.h>
#include <SDL\SDL_image.h>
#include <iostream>
#include <string>
#include <SDL\SDL_ttf.h>
#include <math.h>
#include "Manager.h"
class Circle
{
public:
	Circle(SDL_Renderer *renderer, float xPos, float yPos, float radi, float angle);
	~Circle();

	void drawCircle();

	void move();

	void circleCollision(Circle A, Circle B);

	float m_xPos, m_yPos;
	float m_theta;
	float m_step;
	float m_radi;
	float m_gravity;

	float t;

	double m_velX, m_velY;

	float m_angle;
	float m_speed;
	float m_scaleX, m_scaleY;

private:
	
	SDL_Rect m_imgRect;

	SDL_Renderer *m_gameRenderer;

	SDL_Texture *texture;
};

