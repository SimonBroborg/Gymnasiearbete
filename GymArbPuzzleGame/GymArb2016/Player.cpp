#include "Player.h"
#include <SDL/SDL_image.h>
#include <iostream>

Player::Player(SDL_Renderer* renderer, Sprite &playerTexture,  float framesX, float framesY)
{
	m_gameRenderer = renderer;
	m_velX = 0;
	m_velY = 0;

	//Displays the player on chosen coordinates
	posRect.x = 100;
	posRect.y = 600;

	//Set the width and height of the frames and rects equal to each other
	m_frameWidth = posRect.w = cropRect.w;
	m_frameHeight = posRect.h = cropRect.h;

	m_leftSpeed = -4; //players velocity to the left
	m_rightSpeed = 4; //players velocity to the right
	m_jumpSpeed =250.0f; //the speed or power that the player jumps with
	m_gravity = 400.0f; //gravity which pushes the player downwards

	bJumping = false; //The player is not jumping when the game starts
	bMoving = false; //The player is not moving when the game starts
	bFallThrough = false; //checks if it is possible to fall through block

	playerJump = Mix_LoadWAV("assets/sounds/player_jump.wav");
}


Player::~Player()
{
}

void Player::render(Sprite &playerTexture, SDL_Renderer* renderer)
{
	SDL_QueryTexture(playerTexture.m_texture, NULL, NULL, &posRect.w, &posRect.h);
	playerTexture.render(renderer, posRect.x, posRect.y);
}


void Player::processInput(SDL_Event &evnt, float delta)
{
	bMoving = false;
	if (evnt.type == SDL_KEYDOWN) {

		switch (evnt.key.keysym.scancode) {

		case SDL_SCANCODE_DOWN:
			bFallThrough = true;
			bMoving = true;
			break;

		case SDL_SCANCODE_LEFT:
			m_velX = m_leftSpeed;
			bMoving = true;
			break;

		case SDL_SCANCODE_RIGHT:
			m_velX = m_rightSpeed;
			bMoving = true;
			break;

		case SDL_SCANCODE_SPACE:
			if (bJumping == false && bOnGround == true) {
				bJumping = true;
				m_velY -= m_jumpSpeed;
				bOnGround = false;
				bMoving = true;
				Mix_PlayChannel(-1, playerJump, 0);

			}
			break;
		}
	}
	else if (evnt.type == SDL_KEYUP) {
		switch (evnt.key.keysym.scancode) {

		case SDL_SCANCODE_LEFT:
			if (m_velX < 0) {
				m_velX = 0;
				bMoving = false;
			}
			break;

		case SDL_SCANCODE_RIGHT:
			if (m_velX > 0) {
				m_velX = 0;
				bMoving = false;
			}
			break;

		case SDL_SCANCODE_DOWN:
			bFallThrough = false;
			bMoving = false;
			break;
		}
		
	}
}

void Player::move(float delta, Tile* tiles[])
{
		//the position of the player before any movement calculations
		m_xPos = posRect.x;
		m_yPos = posRect.y;

		
		//moves the player's x coordinate equal to the x velocity times delta
		posRect.x += m_velX;

		//moves the player equal to the velocity
		posRect.y += m_velY * delta;

		//changes the velocity so slowly fall down again
		m_velY += m_gravity * delta;
		

		for (int i = 0; i < TOTAL_TILES; ++i) {

			if ((tiles[i]->getType() >= TILE_BRIDGE)) {

				if (checkCollision(posRect, tiles[i]->getBox())) {
					if (m_yPos + posRect.h <= tiles[i]->getBox().y) {

						if (tiles[i]->getType() == TILE_BRIDGE && bFallThrough == true)
						{
							m_velY += m_gravity * delta;
							bOnGround = false;
							bJumping = true;
						}
						else {
							m_velY = 0;
							posRect.y = tiles[i]->getBox().y - posRect.h;
							bOnGround = true;
							bJumping = false;
						}
					}

					//colliding from left
					else if (m_xPos - tiles[i]->getBox().w >= tiles[i]->getBox().x)
					{
						if (tiles[i]->getType() != TILE_BRIDGE)
							posRect.x = tiles[i]->getBox().x + tiles[i]->getBox().w;
					}

					// colliding from right
					else if (m_xPos + posRect.w <= tiles[i]->getBox().x)
					{
						if(tiles[i]->getType() != TILE_BRIDGE)
							posRect.x = tiles[i]->getBox().x - posRect.w;
					}

					//colliding from the bottom
					else if (m_yPos >= tiles[i]->getBox().y + tiles[i]->getBox().h)
					{
						if (tiles[i]->getType() != TILE_BRIDGE) {
							posRect.y = tiles[i]->getBox().y + tiles[i]->getBox().h;
							m_velY += -m_velY;
						}
					}
				}
			}
		}

		
		

		/*collision checking for each object inside the vectors "rects"
					for (int i = 0; i < rects.size(); i++)
					{
						SDL_Rect &rect = rects[i];
			
						if (checkCollision(posRect, rect))
						{
			
							//colliding from above
							if (m_yPos + posRect.h <= rect.y)
							{
								//checks if the player can fall through the block (stands on a platform)
								if (rect.h == 8 && bFallThrough == true)
								{
									m_velY += m_gravity * delta;
									bOnGround = false;
									bJumping = true;
								}
								else
								{
									bJumping = false;
									m_velY = 0;
									posRect.y = rect.y - posRect.h;
									bOnGround = true;
								}
							}
			
							//colliding from left
							else if (m_xPos - rect.w >= rect.x)
							{
								if (rect.h == 32)
									posRect.x = rect.x + rect.w;
							}
			
							// colliding from right
							else if (m_xPos + posRect.w <= rect.x)
							{
								if (rect.h == 32)
									posRect.x = rect.x - posRect.w;
							}
			
							//colliding from the bottom
							else if (m_yPos >= rect.y + rect.h)
							{
								if (rect.h == 32)
								{
									posRect.y = rect.y + rect.h;
									m_velY += -m_velY;
								}
							}
						}
					}	
			
					if (bJumping)
						std::cout << m_yPos << std::endl;*/

		
}



//Makes sure the player cant go outside the window
void Player::keepInsideBorder()
{
	//makes sure the player cant go outside the window on the left
	if (posRect.x <= 0)
		posRect.x = 0;

	//makes sure the player cant go outside the window on the right
	if (posRect.x + posRect.w >= SCREEN_WIDTH)
		posRect.x = SCREEN_WIDTH - posRect.w;

	//the player cant fall through the bottom of the window
	if (posRect.y + posRect.h >= SCREEN_HEIGHT)
	{
		posRect.y = SCREEN_HEIGHT - posRect.h;
		bJumping = false;
		bOnGround = true;
		m_velY = 0;
	}

	//the player cant move outside the top of the window
	if (posRect.y < 0)
		posRect.y = 0;
}


float Player::getVelX() {
	return m_velX;
}

float Player::getVelY() {
	return m_velY;
}

SDL_Rect Player::getBox() {
	return posRect;
}