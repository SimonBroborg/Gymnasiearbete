#pragma once
#include <SDL\SDL.h>
#include <string>
class Player
{
public:
	Player(SDL_Renderer *renderTarget, std::string filePath, int x, int y, int framesX, int framesY);
	~Player();

	SDL_Rect positionRect;
	void update(float delta, const Uint8 *KEY_STATE);
	void draw(SDL_Renderer *renderTarget);
	

private:
	SDL_Rect _cropRect;
	SDL_Texture *_texture;
	float _moveSpeed;
	float _frameCounter;
	int _frameWidth, _frameHeight;
	int _textureWidth;
	bool _isActive;
	SDL_Scancode keys[4];
};

