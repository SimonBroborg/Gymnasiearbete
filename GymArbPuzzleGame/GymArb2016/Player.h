#pragma once
#include <SDL\SDL.h>
#include <string>
#include <vector>
#include "Sprite.h"
#include "Engine.h"
#include "Tile.h"

class Player : public Engine
{
public:
	//creates the player sprite, x and y position and the number of frames for the player when the game starts
	Player(SDL_Renderer* renderer, Sprite &playerTexture, float framesX, float framesY);
	~Player();

	//Renders the player texture based on the position
	void render(Sprite &playerTexture, SDL_Renderer* renderer);

	//takes the players inputs and calculates the movement of the player
	void processInput(SDL_Event &evnt, float delta);

	//moves the player based on the posRect's x and y coordinates. Also calls the checkCollision() function
	void move(float delta, Tile* tiles[]);

	void keepInsideBorder();

	//Keeps track of the players position
	SDL_Rect posRect;

	float getVelX();
	float getVelY();

	SDL_Rect getBox();

private:
	Mix_Chunk *playerJump;

	//Booleans which checks movement, jumping and if the player is on the ground
	bool bJumping, bMoving, bOnGround, bFallThrough;

	//the x and y velocity
	float m_velX, m_velY;

	//the speed which the player moves at to the left and right
	double m_leftSpeed, m_rightSpeed, m_jumpSpeed;

	//the players gravity
	float m_gravity;


	//Shows the current frame of the player on the sprite sheet
	SDL_Rect cropRect;

	//Checks which the active frame is
	float m_frameCounter;

	//the dimensions of the player. Will help choosing the right part of the players sprite sheet
	float m_frameWidth, m_frameHeight;

	//width of the whole texture
	float m_textureWidth;

	//keeps track of the x and y coordinates before any movement. Used to know from which side the collision is from. 
	float m_xPos, m_yPos;

	float m_windowWidth, m_windowHeight;

	SDL_Renderer* m_gameRenderer; 




};

