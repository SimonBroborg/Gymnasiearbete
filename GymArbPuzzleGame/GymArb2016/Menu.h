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

	

	//Renders the menus buttons and the background
	void showMenu(SDL_Renderer* renderer);

	//Creates a button 
	void createButton(SDL_Renderer* renderer, std::string text);

	//Checks if any button on the menu is hovered
	void checkHover(int mouseX, int mouseY); 
	//Centers the buttons 
	void createMenu(); 

	//True or false depending on if the menu is currently showing
	bool getShowing(); 

	//Sets the isShowing variable
	void setShowing(bool showing);

	void changeActive(int indexChange); 

	//Contains all the buttons
	std::vector<Button> buttons;

	SDL_Texture* buttonTexture;


	SDL_Texture* background; 

private:
	//The space between the buttons
	int buttonSpacing; 

	//The height of the menu
	float menuHeight;

	bool isShowing;
};

