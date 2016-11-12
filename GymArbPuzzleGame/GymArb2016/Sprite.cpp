#include "Sprite.h"
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


Sprite::Sprite()
{

	m_width = 0;
	m_height = 0;
	m_texture = nullptr;
}


Sprite::~Sprite()
{
	//Deallocate
	free();
}

bool Sprite::loadFromFile(std::string path, SDL_Renderer * renderer) {
	//get rid of preexisting texture
	free();
	m_texture = IMG_LoadTexture(renderer, path.c_str());

	SDL_QueryTexture(m_texture, NULL, NULL, &m_width, &m_height);


	return m_texture != nullptr;
}

void Sprite::free() {
	if (m_texture != nullptr) {
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
		m_width = 0;
		m_height = 0;
	}
}


void Sprite::render(int x, int y, SDL_Rect* clip /* = NULL */, double angle /* = 0.0 */, SDL_Point* center /* = NULL */, SDL_RendererFlip flip /* = SDL_FLIP_NONE */) 
{
	//set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, m_width, m_height };

	//set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w = 80;
		renderQuad.h = clip->h = 80;	
	}

	
	//render to screen
	SDL_RenderCopyEx(renderer, m_texture, clip, &renderQuad, angle, center, flip);

	if (m_texture == nullptr) {
		std::cout << "m_texture is nullptr" << std::endl;
	}
}

int Sprite::getWidth()
{
	return m_width;
}

int Sprite::getHeight()
{
	return m_height;
}
/*
void Sprite::loadSprite(SDL_Renderer* renderer, std::string path, float xPosition, float yPosition) {
	//loads the objects sprite texture
	m_texture = IMG_LoadTexture(renderer, path.c_str());
	if (m_texture != nullptr)
		std::cout << "Successfully loaded sprite from path: " << path << std::endl;
	else
		std::cout << "Could not load sprite from path: " << path << std::endl;

	//sets the starting x and y position of the sprite
	xPos = xPosition; 
	yPos = yPosition;
}
*/