#pragma once
#include <SDL\SDL.h>
#include <string>
#include <vector>
#include "Engine.h"
#include "Sprite.h"

class Button : public Engine
{
public:
	Button(std::string path, SDL_Renderer * renderer);
	~Button();

	//Render the button
	void render(SDL_Renderer* renderer);

	//Get the button box
	SDL_Rect getBox();

	//Set x and y coords for the button
	void setX(int x); 
	void setY(int y);

	void setActive(bool active);

	bool getActive();

	//Check if mouse is hovering the button
	bool checkHover(int mouseX, int mouseY);

	

	SDL_Texture* buttonTexture; 

	void setTexture(std::string path, SDL_Renderer * renderer);
private:
	SDL_Rect m_box;

	//Flag for hovering
	bool hovering;

};

