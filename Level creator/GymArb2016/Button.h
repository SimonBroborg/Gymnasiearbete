#pragma once
#include <SDL\SDL.h>
#include <string>
#pragma once
#include <SDL\SDL.h>
#include <string>
#include <vector>
#include "Engine.h"
#include "Sprite.h"

class Button : public Engine
{
public:
	Button(SDL_Renderer * renderer, std::string text);
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

	Sprite buttonText;

	SDL_Texture* buttonTextTexture;

	SDL_Texture* buttonTexture;

	void setImage(SDL_Renderer* renderer, std::string filePath);

	void setText(SDL_Renderer* renderer, std::string text);
private:
	SDL_Rect m_box;

	//Flag for hovering
	bool hovering;

	TTF_Font *font;
	float fontSize;

	int buttonWidth;
	int buttonHeight;

	SDL_Color color; 
};

