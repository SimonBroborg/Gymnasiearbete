#include "Sprite.h"
#include "Manager.h"
#include <iostream>


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

bool Sprite::loadFromFile(SDL_Renderer* renderer, std::string path) {
	free();
	gameRenderer = renderer;

	m_texture = IMG_LoadTexture(renderer, path.c_str());
	m_height = 80;
	m_width = 80;
	SDL_Rect test = { 100, 100, 100, 100 };
	SDL_RenderCopy(gameRenderer, m_texture, &test, NULL);

	return m_texture != nullptr;
}


void Sprite::render(int x, int y, SDL_Rect* clip /* = NULL */, double angle /* = 0.0 */, SDL_Point* center /* = NULL */, SDL_RendererFlip flip /* = SDL_FLIP_NONE */) 
{
	//set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, m_width, m_height};

	//set clip rendering dimensions
	if (clip != nullptr) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//render to screen
	SDL_RenderCopyEx(gameRenderer, m_texture, clip, &renderQuad, angle, center, flip);
}

int Sprite::getWidth()
{
	return m_width;
}

int Sprite::getHeight()
{
	return m_height;
}

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


void Sprite::free() {
	if (m_texture != nullptr) {
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
		m_width = 0;
		m_height = 0;
	}
}