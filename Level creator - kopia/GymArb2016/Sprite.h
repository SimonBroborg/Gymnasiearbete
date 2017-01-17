#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>
#include "Engine.h"

class Sprite : public Engine
{
public:
	
	Sprite();
	~Sprite();

	void free();

	//loads the sprite and sets its starting position
	void loadSprite(SDL_Renderer* renderer, std::string path, float xPosition, float yPosition);

	SDL_Renderer* gameRenderer;


	bool loadFromFile(std::string path, SDL_Renderer* renderer);
	
	void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);


	//gets image dimensions
	int getWidth();
	int getHeight();

	
	//the sprite texture
	SDL_Texture* m_texture;

private:
	

	//the texture dimensions
	int m_width;
	int m_height;
	
};

