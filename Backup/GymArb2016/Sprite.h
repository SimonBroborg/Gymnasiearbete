#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>
class Sprite
{
public:
	Sprite();
	~Sprite();

	void free();

	//loads the sprite and sets its starting position
	void loadSprite(SDL_Renderer* renderer, std::string path, float xPosition, float yPosition);

	float xPos, yPos;
	float m_spriteWidth;
	float m_spriteHeight;
	SDL_Renderer* gameRenderer;


	bool loadFromFile(SDL_Renderer* renderer, std::string path);
	
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	int getWidth();
	int getHeight();
protected:
	float screenHeight;
	float screenWidth;
	SDL_Texture* m_texture;

	

private:

	int m_width;
	int m_height;
	
};

