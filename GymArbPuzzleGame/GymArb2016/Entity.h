#pragma once
#include <SDL\SDL.h>
#include <SDL\SDL_image.h>
#include <SDL\SDL_ttf.h>

#include <string>
#include <iostream>

class Entity
{
public:
	Entity(SDL_Renderer* renderer);
	~Entity();

	//Destroys the entity
	void free();

	//Renders the entity
	void render(int xPos, int yPos, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Setters
	void setSprite(std::string path); 
	
	////Getters
	//Get the collision box
	SDL_Rect getBox();

	//Get the texture
	SDL_Texture* getSprite();

	//Get width
	int getWidth();

	//Get the height
	int getHeight();

	//Get the x position
	int getX();

	//Get the y position
	int getY();

	//Get the angle
	float getAngle();

private:

	//// SDL Variables ////
	//Texture for the entity
	SDL_Texture* m_texture;

	//Renderer for the entity
	SDL_Renderer* m_renderer;

	//Position/Collision rectangle
	SDL_Rect m_posRect; 

	//// Other variables ////
	//The entity's angle
	float m_angle; 
};


