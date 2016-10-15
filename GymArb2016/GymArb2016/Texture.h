#pragma once
#include <SDL\SDL.h>
#include <SDL\SDL_ttf.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include "GameManager.h"
class Texture
{
public:
	Texture();
	~Texture();

	bool loadFromRenderedText(std::string textureText, SDL_Renderer* renderer, SDL_Color textColor);

	SDL_Texture* getTexture();

	float getHeight();
	float getWidth();

	SDL_Texture* loadTexture(std::string path, SDL_Renderer* renderer);

	void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);
private:

	SDL_Texture* _texture;
	float _width;
	float _height;

	TTF_Font* _font;
};

