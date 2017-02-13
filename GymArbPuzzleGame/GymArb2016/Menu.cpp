#include "Menu.h"
#include <cstdio>
#include <iostream>


Menu::Menu(SDL_Renderer* renderer, std::string path)
{
	background = IMG_LoadTexture(renderer, path.c_str());
	SDL_SetTextureBlendMode(background, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(background, 150);

	buttonSpacing = 10;
	menuHeight = 0; 

	isShowing = false;
}


Menu::~Menu()
{
}

void Menu::createButton(SDL_Renderer* renderer, std::string text)
{
	
	Button * button = new Button(renderer, text.c_str());
	buttons.push_back(*button);
}

void Menu::showMenu(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, background, NULL, NULL);

	for (int i = 0; i < buttons.size(); i++) {
		buttons[i].render(renderer);
	}	
	isShowing = true;

}

void Menu::createMenu() {
	//Set the height of the menu
	for (int i = 0; i < buttons.size(); i++) {
		menuHeight += buttons[i].getBox().h; //Add together all the buttons heights
	}
	//Add the button spacing to the height
	menuHeight += buttonSpacing * (buttons.size() - 1);

	buttons[0].setY(SCREEN_HEIGHT / 2 - menuHeight / 2);
	
	for (int i = 1; i < buttons.size(); i++) {
		buttons[i].setY(buttons[i - 1].getBox().y + buttons[i-1].getBox().h + buttonSpacing);
	}

}

void Menu::checkHover(int mouseX, int mouseY) {
	for (int i = 0; i < buttons.size(); i++) {
		buttons[i].checkHover(mouseX, mouseY);
	}
}

bool Menu::getShowing() {
	return isShowing;
}

void Menu::setShowing(bool showing) {
	isShowing = showing;
}

void Menu::changeActive(int indexChange) {
	bool buttonActive = false; 
	int newActive; 

	for (int i = 0; i < buttons.size(); i++) {
		if (buttons[i].getActive()) {
			buttonActive = true; 
			
			newActive = i + indexChange;
			if (newActive == buttons.size())
				newActive = 0;
			else if (newActive < 0)
				newActive = buttons.size() - 1;
			buttons[i].setActive(false);
			buttons[newActive].setActive(true);
			break;
		}
	}
	if(!buttonActive)
		buttons[0].setActive(true);
}