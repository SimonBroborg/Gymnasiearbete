#include "Player.h"
#include <SDL\SDL_image.h>
#include <iostream>


Player::Player(SDL_Renderer *renderTarget, std::string filePath, int x, int y, int framesX, int framesY)
{


	SDL_Surface *surface = IMG_Load(filePath.c_str());
	if (surface == nullptr)
		std::cout << "Could not load image to surface! Error: " << SDL_GetError() << std::endl;
	else
	{
		//SDL_SetColorKey(s	urface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 0));
		_texture = SDL_CreateTextureFromSurface(renderTarget, surface);
		if (_texture == nullptr)
			std::cout << "Could not create texture from surface! SDL Error: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(surface);

	SDL_QueryTexture(_texture, NULL, NULL, &_cropRect.w, &_cropRect.h);
	std::cout << _cropRect.w << " " << _cropRect.h;
	positionRect.x = x;
	positionRect.y = y;

	_textureWidth = _cropRect.w;
	_cropRect.w /= framesX;
	_cropRect.h /= framesY;

	_frameWidth = positionRect.w = _cropRect.w;
	_frameHeight = positionRect.h = _cropRect.h;

	_isActive = false;

	static int playerNumber = 0;
	playerNumber++;

	if (playerNumber == 1)
	{
		keys[0] = SDL_SCANCODE_UP;
		keys[1] = SDL_SCANCODE_DOWN;
		keys[2] = SDL_SCANCODE_LEFT;
		keys[3] = SDL_SCANCODE_RIGHT;
	}
	else
	{
		keys[0] = SDL_SCANCODE_W;
		keys[1] = SDL_SCANCODE_S;
		keys[2] = SDL_SCANCODE_A;
		keys[3] = SDL_SCANCODE_D;
	}

	_moveSpeed = 200.0f;	
} 


Player::~Player()
{
	SDL_DestroyTexture(_texture);
}


void Player::update(float delta, const Uint8 *KEY_STATE)
{
	_isActive = true;
	if (KEY_STATE[keys[0]])
	{
		positionRect.y -= _moveSpeed * delta;
		_cropRect.y = _frameHeight * 3;
	}
	else if (KEY_STATE[keys[1]])
	{
		positionRect.y += _moveSpeed * delta;
		_cropRect.y = 0;
	}
	else if (KEY_STATE[keys[2]])
	{
		positionRect.x -= _moveSpeed * delta;
		_cropRect.y = _frameHeight;
	}
	else if (KEY_STATE[keys[3]])
	{
		positionRect.x += _moveSpeed * delta;
		_cropRect.y = _frameHeight * 2;
	}
	else
		_isActive = false;

	if (_isActive)
	{
		_frameCounter += delta;

		if (_frameCounter >= 0.25f)
		{
			_frameCounter = 0;
			_cropRect.x += _frameWidth;
			if (_cropRect.x >= _textureWidth)
				_cropRect.x = 0;
		}
	}
	else
	{
		_frameCounter = 0;
		_cropRect.x = _frameWidth;
	}
}


void Player::draw(SDL_Renderer *renderTarget)
{
	SDL_RenderCopy(renderTarget, _texture, &_cropRect, &positionRect);
}