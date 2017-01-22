#include "Button.h"
#include <iostream>

Button::Button(std::string path, SDL_Renderer* renderer)
{
	buttonTexture = IMG_LoadTexture(renderer, path.c_str());
	SDL_QueryTexture(buttonTexture, NULL, NULL, &m_box.w, &m_box.h);
	m_box.x = SCREEN_WIDTH / 2 - m_box.w / 2;

	SDL_SetTextureBlendMode(buttonTexture, SDL_BLENDMODE_BLEND);

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
	SDL_RenderCopy(renderer, buttonTexture, NULL, &m_box);
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