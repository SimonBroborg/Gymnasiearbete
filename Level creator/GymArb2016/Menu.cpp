#include "Menu.h"
#include <cstdio>
#include <iostream>


Menu::Menu(SDL_Renderer* renderer, std::string path)
{
	background = IMG_LoadTexture(renderer, path.c_str());
	SDL_SetTextureBlendMode(background, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(background, 150);
}


Menu::~Menu()
{
}

void Menu::createButton(SDL_Renderer* renderer, std::string path, int y)
{
	buttonTexture = IMG_LoadTexture(renderer, path.c_str());
	Button * button = new Button(buttonTexture, 0, y);
	buttons.push_back(*button);
}

void Menu::showMenu(Sprite &buttonTexture, SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, background, NULL, NULL);

	for (int i = 0; i < buttons.size(); i++) {
		buttons[i].render(renderer);
	}
	bIsRunning = false;
}