#pragma once
#include <SDL\SDL.h>
#include <string>
#include <vector>
#include "Engine.h"
#include "Sprite.h"

class Button : public Engine
{
public:
	Button(SDL_Texture* texture, int x, int y);
	~Button();

	void render(SDL_Renderer* renderer);

	SDL_Rect getBox();

	bool checkHover(int mouseX, int mouseY);

	void setImage(SDL_Renderer * renderer, std::string filePath); 

	SDL_Texture* buttonTexture;
private:
	SDL_Rect m_box;

	bool hovering;

};

