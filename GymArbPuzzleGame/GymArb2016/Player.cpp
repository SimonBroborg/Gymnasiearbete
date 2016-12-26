#include "Player.h"
#include <SDL/SDL_image.h>
#include <iostream>

Player::Player(SDL_Renderer* renderer, Sprite &playerTexture,  float framesX, float framesY)
{
	SDL_Surface *surface = IMG_Load("assets/player/doctorSS.png");
	if (surface == nullptr)
		std::cout << "Could not load image to surface! Error: " << SDL_GetError() << std::endl;
	else
	{
		//SDL_SetColorKey(s	urface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 0));
		m_texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (m_texture == nullptr)
			std::cout << "Could not create texture from surface! SDL Error: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(surface);

	m_gameRenderer = renderer;
	m_velX = 0;
	m_velY = 0;

	/*//Displays the player on chosen coordinates
	posRect.x = 100;
	posRect.y = 600;*/
	SDL_QueryTexture(m_texture, NULL, NULL, &cropRect.w, &cropRect.h);
	
	
	//Set the width and height of the frames and rects equal to each other
	m_textureWidth = cropRect.w; 
	cropRect.w /= framesX;
	cropRect.h /= framesY;
	
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
	posRect.x = 0;
	posRect.y = 0;
}


Player::~Player()
{
}

void Player::render(Sprite &playerTexture, SDL_Renderer* renderer, int camX, int camY)
{
	//SDL_QueryTexture(m_texture, NULL, NULL, &posRect.w, &posRect.h); 
	
	
	playerTexture.render(renderer, posRect.x - camX, posRect.y- camY, &cropRect);
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
		case SDL_SCANCODE_S:
			bFallThrough = true;
			bMoving = true;
			break;

		case SDL_SCANCODE_LEFT:
			m_velX = m_leftSpeed;
			bMoving = true;
			break;
		case SDL_SCANCODE_A:
			m_velX = m_leftSpeed;
			bMoving = true;
			break;

		case SDL_SCANCODE_RIGHT:
			m_velX = m_rightSpeed;
			bMoving = true;
			break;
		case SDL_SCANCODE_D:
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
		case SDL_SCANCODE_A:
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
		case SDL_SCANCODE_D:
			if (m_velX > 0) {
				m_velX = 0;
				bMoving = false;
			}
			break;

		case SDL_SCANCODE_DOWN:
			bFallThrough = false;
			bMoving = false;
			break;
		case SDL_SCANCODE_S:
			bFallThrough = false;
			bMoving = false;
			break;
		}
		
	}


	
}

void Player::move(float delta, Tile* tiles[], bool &nextLevel)
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
		
		if (bMoving)
		{
			if (m_velX > 0) {
				cropRect.y = m_frameHeight * 2;
			}
			else if (m_velX < 0) {
				cropRect.y = m_frameHeight;
			}

			m_frameCounter += delta;

			if (m_frameCounter >= 0.2f)
			{
				m_frameCounter = 0;
				cropRect.x += m_frameWidth;
				if (cropRect.x >= m_textureWidth)
					cropRect.x = 0;
			}
		}
		else
		{
			m_frameCounter = 0;
			cropRect.x = 0;
		} 

		//collision checking with the map
		for (int i = 0; i < TOTAL_TILES; ++i) {

			//checks collision if the tile type != 0 ( empty space ) 
			if ((tiles[i]->getType() != 0)) {

				//returns true if there is a collision between the player and the tile
				if (checkCollision(posRect, tiles[i]->getBox())) {

					//goes to the next level if the tile is a portal
					if (tiles[i]->getType() == TILE_PORTAL) {
						nextLevel = true;
					}
					//checks if the player collides from the top of the tile
					else if (m_yPos + posRect.h <= tiles[i]->getBox().y) {

						//if the tile is a bridge and "down" is pressed, the player will fall through
						if (tiles[i]->getType() == TILE_BRIDGE && bFallThrough == true)
						{
							m_velY += m_gravity * delta;
							bOnGround = false;
							bJumping = true;
						}
						//if the tile is not a bridge
						else {
							//stops the player from falling
							m_velY = 0;
							posRect.y = tiles[i]->getBox().y - posRect.h;
							bOnGround = true;
							bJumping = false;

							//if the tile is a moving platform, the player will get the same speed
							if (tiles[i]->getType() == TILE_MOVING_PLATFORM) {
									if (!bMoving)
										m_velX = tiles[i]->getSpeed();
							}
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
						//if the tile is no a bridge the player will collide 
						if(tiles[i]->getType() != TILE_BRIDGE)
							posRect.x = tiles[i]->getBox().x - posRect.w;
						
					}

					//colliding from the bottom
					else if (m_yPos >= tiles[i]->getBox().y + tiles[i]->getBox().h)
					{
						// if the tile is not a bridge the player will collide 
						if (tiles[i]->getType() != TILE_BRIDGE) {
							posRect.y = tiles[i]->getBox().y + tiles[i]->getBox().h;
							m_velY += -m_velY;
						}
						
					}
				}
			}
		}

}


//Makes sure the player cant go outside the window
void Player::keepInsideBorder()
{
	//makes sure the player cant go outside the window on the left
	if (posRect.x <= 0)
		posRect.x = 0;

	//makes sure the player cant go outside the window on the right
	if (posRect.x + posRect.w >= LEVEL_WIDTH)
		posRect.x = LEVEL_WIDTH - posRect.w;

	//the player cant fall through the bottom of the window
	if (posRect.y + posRect.h >= LEVEL_HEIGHT)
	{
		posRect.y = LEVEL_HEIGHT - posRect.h;
		bJumping = false;
		bOnGround = true;
		m_velY = 0;
	}

	//the player cant move outside the top of the window
	if (posRect.y < 0)
		posRect.y = 0;
}

//setters and getters for velocity and collision box
float Player::getVelX() {
	return m_velX;
}

float Player::getVelY() {
	return m_velY;
}

SDL_Rect Player::getBox() {
	return posRect;
}