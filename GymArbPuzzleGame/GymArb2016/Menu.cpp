#include "Menu.h"



Menu::Menu()
{
}


Menu::~Menu()
{
}

void Menu::createMenu()
{
	Button* exitButton = new Button(0, 0, 100, 100);
	buttons[0] = exitButton;

	Button * levelButton = new Button(SCREEN_WIDTH/ 2 - 50, 150, 100, 100);
	buttons[1] = levelButton;

}

void Menu::showMenu(Sprite &buttonTexture, SDL_Renderer* renderer)
{
	for (int i = 0; i < TOTAL_MENU_BUTTONS; i++) {
		buttons[i]->render(buttonTexture, renderer);

	}
	
}

void Menu::checkHover(int mouseX, int mouseY)
{
	for (int i = 0; i < TOTAL_MENU_BUTTONS; i++) {
		buttons[i]->checkHover(mouseX, mouseY);
	}
}