#include "Button.h"
#include <iostream>

Button::Button(SDL_Renderer* renderer, std::string text)
{
	buttonTexture = IMG_LoadTexture(renderer, "assets/buttons/gameButton.png");
	SDL_QueryTexture(buttonTexture, NULL, NULL, &m_box.w, &m_box.h);
	m_box.x = SCREEN_WIDTH / 2 - m_box.w / 2;

	SDL_SetTextureBlendMode(buttonTexture, SDL_BLENDMODE_BLEND);

	SDL_Color color = { 0, 0, 0, 0xFF };
	fontSize = 30; 

	font = TTF_OpenFont("assets/fonts/OpenSans-Italic.ttf", fontSize);
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (textSurface != nullptr)
	{
		//Create texture from surface pixels
		buttonTextTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (buttonTextTexture == nullptr)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			buttonWidth = textSurface->w;
			buttonHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}

}

Button::~Button()
{
}

SDL_Rect Button::getBox()
{
	return m_box;
}

void Button::render(SDL_Renderer* renderer)
{
	SDL_Rect buttonBox;
	buttonBox.w = buttonWidth;
	buttonBox.h = buttonHeight; 
	buttonBox.x = m_box.x + m_box.w / 2 - buttonBox.w / 2; 
	buttonBox.y = m_box.y + m_box.h / 2 - buttonBox.h / 2;

	SDL_RenderCopy(renderer, buttonTexture, NULL, &m_box);
	SDL_RenderCopy(renderer, buttonTextTexture, NULL, &buttonBox);
}

bool Button::checkHover(int mouseX, int mouseY)
{
	hovering = false;
	if ((mouseX >= m_box.x) && (mouseX <= m_box.x + m_box.w) && (mouseY >= m_box.y) && (mouseY <= m_box.y + m_box.h)) {
		
		hovering = true;
	}
	if (hovering)
		setActive(true);
	else
		setActive(false);
	
	
	return hovering;
}

void Button::setX(int x) {
	m_box.x = x;
}

void Button::setY(int y) {
	m_box.y = y;
}

bool Button::getActive() {
	return hovering; 
}

void Button::setTexture(std::string path, SDL_Renderer * renderer) {
	
	buttonTexture = IMG_LoadTexture(renderer, path.c_str());
}

void Button::setActive(bool active) {
	if (active) {
		SDL_SetTextureAlphaMod(buttonTexture, 150);
	}
	else {
		SDL_SetTextureAlphaMod(buttonTexture, 255);
	}

	hovering = active;
}
