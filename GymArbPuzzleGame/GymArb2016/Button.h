#pragma once
#include <SDL\SDL.h>
#include <string>
#include <vector>
#include "Engine.h"
#include "Sprite.h"

class Button
{
public:
	Button(int x, int y, int width, int height);
	~Button();

	void render(Sprite &buttonTexture, SDL_Renderer* renderer);

	SDL_Rect getBox();

	bool checkHover(int mouseX, int mouseY);
private:
	SDL_Rect m_box;

	bool hovering;

};

