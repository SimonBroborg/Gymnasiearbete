#include "Button.h"
#include <iostream>

Button::Button(int x, int y, int width, int height)
{
	m_box.x = x;
	m_box.y = y;

	m_box.w = width;

	m_box.h = height;
}


Button::~Button()
{
}

SDL_Rect Button::getBox()
{
	return m_box;
}

void Button::render(Sprite &buttonTexture, SDL_Renderer* renderer)
{
	buttonTexture.render(renderer, m_box.x, m_box.y);
}

bool Button::checkHover(int mouseX, int mouseY)
{
	hovering = false;
	if ((mouseX >= m_box.x) && (mouseX <= m_box.x + m_box.w) && (mouseY >= m_box.y) && (mouseY <= m_box.y + m_box.h)) {
		std::cout << "Hovering" << std::endl;
		hovering = true;
	}
	
	
	return hovering;
}