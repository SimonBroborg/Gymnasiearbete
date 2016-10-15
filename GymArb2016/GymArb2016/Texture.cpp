#include "Texture.h"



Texture::Texture()
{
	_font = TTF_OpenFont("sample.ttf", 10);
	_texture = nullptr;

	_width = 0;
	_height = 0;
}


Texture::~Texture()
{
}

bool Texture::loadFromRenderedText(std::string textureText, SDL_Renderer* renderer, SDL_Color textColor)
{
	SDL_Surface* textSurface = TTF_RenderText_Solid(_font, textureText.c_str(), textColor);
	if (textSurface == nullptr)
	{
		std::cout << "Unable to render text to surface! SDL_ttf error: " << TTF_GetError();
		return 1;
	}
	else
	{
		_texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (_texture == nullptr)
		{
			std::cout << "Unable to created texture from surface! SDL Error: " << SDL_GetError();
			return 1;
		}
		else
		{
			_width = textSurface->w;
			_height = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}

	return _texture != nullptr;
}


SDL_Texture* Texture::getTexture()
{
	return _texture;
}

float Texture::getHeight()
{
	return _height;
}

float Texture::getWidth()
{
	return _width;
}

SDL_Texture* Texture::loadTexture(std::string path, SDL_Renderer* renderer)
{
	_texture = IMG_LoadTexture(renderer, path.c_str());
	if (_texture == nullptr)
	{
		std::cout << "Failed to load texture from path! SDL error: ", SDL_GetError();
		return nullptr;
	}
	return _texture;
}

void Texture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, 100, 100 };

	//Set clip rendering dimensions
	if (clip != nullptr)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	else
	{
		std::cout << "failed!", SDL_GetError();
	}

	//Render to screen
	SDL_RenderCopyEx(renderer, _texture, clip, &renderQuad, angle, center, flip);
}