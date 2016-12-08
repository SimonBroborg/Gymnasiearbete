#pragma once
#include "Sprite.h"
#include <SDL\SDL.h>
#include <string>
#include <vector>
#include "Engine.h"
#include "Sprite.h"
#include "Button.h"
class Menu : public Engine
{
public:
	Menu(SDL_Renderer* renderer, std::string path);
	~Menu();

	static const int TOTAL_MENU_BUTTONS = 2;

	void showMenu(Sprite &buttonTexture, SDL_Renderer* renderer);
	void createButton(SDL_Renderer* renderer, std::string path, int x);

	std::vector<Button> buttons;

	SDL_Texture* buttonTexture;


	SDL_Texture* background; 
};

