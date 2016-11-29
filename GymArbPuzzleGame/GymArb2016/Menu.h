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
	Menu();
	~Menu();

	static const int TOTAL_MENU_BUTTONS = 2;

	void showMenu(Sprite &buttonTexture, SDL_Renderer* renderer);
	void createMenu();
	void checkHover(int mouseX, int mouseY);

	Button* buttons[TOTAL_MENU_BUTTONS];
};

