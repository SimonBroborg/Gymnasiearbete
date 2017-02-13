#include "Sprite.h"
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>


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
	SDL_Surface *tempSurface = IMG_Load(path.c_str());
	if (tempSurface == nullptr) {
		std::cout << "Could not load surface!" << std::endl;
	}
	else {
		SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 0xdb, 0xdb, 0xdb));
		m_texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
		SDL_QueryTexture(m_texture, NULL, NULL, &m_width, &m_height);
	}

	

	SDL_FreeSurface(tempSurface);
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


void Sprite::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip /* = NULL */, double angle /* = 0.0 */, SDL_Point* center /* = NULL */, SDL_RendererFlip flip /* = SDL_FLIP_NONE */) 
{
	xPos = x;
	yPos = y; 
	//set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, m_width, m_height };

	//set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	//render to screen
	SDL_RenderCopyEx(renderer, m_texture, clip, &renderQuad, angle, center, flip);

}

int Sprite::getWidth()
{
	return m_width;
}

int Sprite::getHeight()
{
	return m_height;
}


bool Sprite::loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Renderer* renderer) {
	
	font = TTF_OpenFont("assets/fonts/fixedsys.ttf", fontSize);
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	if (textSurface != nullptr)
	{
		//Create texture from surface pixels
		m_texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (m_texture == nullptr)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			m_width = textSurface->w;
			m_height = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}


	//Return success
	return m_texture != NULL;
}

void Sprite::setFontSize(float size) {
	fontSize = size;
}

float Sprite::getFontSize() {
	return fontSize;
}

int Sprite::getX() {
	return xPos;
}

int Sprite::getY() {
	return yPos;
}