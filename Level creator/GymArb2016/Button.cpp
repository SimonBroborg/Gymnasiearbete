#include "Button.h"
#include <iostream>

Button::Button(SDL_Texture* texture, int x, int y)
{
	SDL_QueryTexture(texture, NULL, NULL, &m_box.w, &m_box.h);
	m_box.x = SCREEN_WIDTH / 2 - m_box.w / 2;
	m_box.y = y;
	buttonTexture = texture;
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

	return hovering;
}

void Button::setImage(SDL_Renderer* renderer, std::string filePath) {
	buttonTexture = IMG_LoadTexture(renderer, filePath.c_str());
}