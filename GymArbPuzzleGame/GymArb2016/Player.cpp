//////////////////////////// ALL INCLUDED FILES ////////////////////////////////////////////////////////////
#include "Player.h"
#include <SDL/SDL_image.h>
#include <iostream>

///////////////////////// CONSTRUCTOR /////////////////////////////////////////////////
Player::Player(SDL_Renderer* renderer, Sprite &playerTexture, float framesX, float framesY)
{
	/////////////////////////// CREATES THE PLAYER SPRITE SHEET
	SDL_Surface *surface = IMG_Load("assets/player/player.png"); //Creates a surface of the sprite sheet
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

	////////////////////////// INITIALIZES VARIABLES ////////////////////////////////////////////////////////////
	//floats and doubles
	m_velX = 0; //Starting with 0 horizontal velocity
	m_velY = 0; //Starting with 0 vertical velocity
	m_maxHorVel = 200;

	m_jumpHeight = 8;
	m_timeToJumpApex = .4f;


	m_gravity = 600;// (2 / m_jumpHeight) / pow(m_timeToJumpApex, 2);
	m_jumpVelocity = 400;// abs(m_gravity) * m_timeToJumpApex;
	m_acceleration = 1.5;

	//SDL
	posRect.x = 0; //Starting on x = 0
	posRect.y = 0; //Starting on y = 0

				   //Bools
	bJumping = false; //The player is not jumping when the game starts
	bMoving = false; //The player is not moving when the game starts
	bFallThrough = false; //checks if it is possible to fall through block
	bSprint = false;
	onMovingPlatform = false;
	bOnGround = false;

	//Sounds
	playerJump = Mix_LoadWAV("assets/sounds/player_jump.wav");
	playerSaw = Mix_LoadWAV("assets/sounds/sawDeath.wav");

	/////////////////////// PREPARES FOR MOVEMENT ANIMATIONS ///////////////////////////////////////////////////////

	SDL_QueryTexture(m_texture, NULL, NULL, &cropRect.w, &cropRect.h); //cropRect's dimensions is equal to the sprite sheet
	m_textureWidth = cropRect.w; //Sets the texture width equal to the sprite sheet's width
	m_frameWidth = posRect.w = cropRect.w /= framesX; //Sets the width of each player frame
	m_frameHeight = posRect.h = cropRect.h /= framesY; //Sets the height of each player frame
}

////////////////////////////////////// DESSTRUCTOR /////////////////////////////////////////////////////////
Player::~Player()
{
}

///////////////////////////////////// READS INPUT FROM THE PLAYER ////////////////////////////////////////////////////

void Player::Update(SDL_Event &evnt, float delta)
{

	///////////////////////////////////// KEY DOWN EVENTS //////////////////////////////////////////////

	if (evnt.type == SDL_KEYDOWN && evnt.key.repeat == 0) {

		//Checks which key is pressed
		switch (evnt.key.keysym.sym) {

		case SDLK_LSHIFT: //If left shift is pressed
			if (bOnGround) //If the player is on the ground
				bSprint = true; //The player tries to run
			break;


		case SDLK_DOWN: //If the down key is pressed
			bFallThrough = true; //Try if player can fall through the current tile
			break;

		case SDLK_LEFT: //If the left key is pressed
			m_velX -= m_maxHorVel; //Sets the horizontal speed 
			bMoving = true; //The player is moving
			break;

		case SDLK_RIGHT: //If the right key is pressed
			m_velX += m_maxHorVel; //Sets the horizontal speed
			bMoving = true; //The player is moving
			break;

		case SDLK_SPACE: //If space is pressed
			if (bJumping == false && bOnGround == true) { //If the player is on the ground and not already jumping
				bJumping = true; //The player is jumping
				m_velY -= m_jumpVelocity + m_velY; //Sets the horizontal speed
				bOnGround = false; //The player is not on the ground
				bMoving = true; //The player is moving 
				Mix_PlayChannel(-1, playerJump, 0); //Plays jump sound
			}
			break;
		}
	}
	//////////////////////////////// KEY UP EVENTS ///////////////////////////////////////////

	else if (evnt.type == SDL_KEYUP && evnt.key.repeat == 0) {

		//Checks which key is released   
		switch (evnt.key.keysym.sym) {

		case SDLK_LSHIFT:
			bSprint = false;
			break;

			//If the left key is released
		case SDLK_LEFT:
			m_velX += m_maxHorVel; //Sets horizontal speed to 0
			bMoving = false; //The player is not moving
			break;

			//If the right key is released
		case SDLK_RIGHT:
			m_velX -= m_maxHorVel; //Sets the horizontal speed to 0
			bMoving = false; //The player is not moving
			break;

			//If the down key is released
		case SDLK_DOWN:
			bFallThrough = false; //The player no longer wants to fall through a tile
			bMoving = false; //The player is not moving
			break; 
		}
	}
}

///////////////////////////////////// MOVEMENT //////////////////////////////////////////////////////////////////

void Player::Move(float delta, Tile* tiles[], bool &nextLevel)
{
	///////////////////// NEW VALUES TO VARIABLES /////////////////////////////////////////
	m_xPos = posRect.x; //Saves horizontal position before movement
	m_yPos = posRect.y; //Saves vertical position before movement

	posRect.x += m_velX * delta; //Changes horziontal position
	posRect.y += m_velY * delta; //Changes vertical position

	m_velY += m_gravity * delta; //Applies gravity to the vertical speed
	////////////////////////////////// COLLISION CHECKING /////////////////////////////////////////////////////////
	for (int i = 0; i < TOTAL_TILES; ++i) { //Loops through all tiles
		if ((tiles[i]->getType() != TILE_NONE)) { //Checks collision if the tile type != 0 ( empty space ) 
			
				/////////////////////////// SAW COLLISION /////////////////////////////////////////////

			/*if (tiles[i]->getType() == TILE_SAW_1) { //If the tile is a saw
				if (circularCollision(tiles[i]->getBox(), posRect)) { //Checks circular collision with the saw
					Mix_PlayChannel(-1, playerSaw, 0); //Plays saw sound if there is a collision
					Respawn(); //Respawns the player
				}
			}*/
			////////////////////////// COLLISION WITH A TILE //////////////////////////////////////////////////////////7
			if (checkCollision(posRect, tiles[i]->getBox())) { //If there's a collision with the tile
															/////////////////////// PORTAL COLLISION /////////////////////////////
				/*if (tiles[i]->getType() == TILE_PORTAL) {
					nextLevel = true; //Changes to the next level
				}
				///////////////////// SPIKE COLLISION //////////////////////////////
				if (tiles[i]->getType() == TILE_SPIKES) {
					Respawn(); //The player dies and respawns
				}*/

				////////////////// 'NORMAL' TILE COLLISION /////////////////////////
				//checks if the player collides from the top of the tile
				if (m_yPos + posRect.h <= tiles[i]->getBox().y) {
					
					/*//if the tile is a bridge and "down" is pressed, the player will fall through
					if (tiles[i]->getType() == TILE_BRIDGE && bFallThrough == true)
					{
						m_velY += m_gravity * delta; //Falls through the tile
						bOnGround = false; //The player is not on the ground
						bJumping = true;
					}*/
					
					//if the tile is not a bridge
					
						//stops the player from falling
						posRect.y = tiles[i]->getBox().y - posRect.h;
						bOnGround = true;
						bJumping = false;
						m_velY = 0;

						/*if (tiles[i]->getType() >= TILE_ICE_WHOLE && tiles[i]->getType() <= TILE_ICE_BROKEN_3) {
							tiles[i]->destroy(delta);
						}

						//if the tile is a moving platform, the player will get the same speed
						if (tiles[i]->getType() == TILE_MOVING_PLATFORM) {
							onMovingPlatform = true;
						}
						else {
							onMovingPlatform = false;
						}*/
				}

				//colliding from left
				else if (m_xPos - tiles[i]->getBox().w >= tiles[i]->getBox().x)
				{
					if (tiles[i]->getType() != TILE_BRIDGE) {
						if (tiles[i]->getType() != TILE_MOVING_PLATFORM)
							posRect.x = tiles[i]->getBox().x + tiles[i]->getBox().w;
					}

				}

				// colliding from right
				else if (m_xPos + posRect.w <= tiles[i]->getBox().x)
				{
					//if the tile is no a bridge the player will collide 
					if (tiles[i]->getType() != TILE_BRIDGE) {
						if (tiles[i]->getType() != TILE_MOVING_PLATFORM)
							posRect.x = tiles[i]->getBox().x - posRect.w;
					}

				}

				//colliding from the bottom
				else if (m_yPos >= tiles[i]->getBox().y + tiles[i]->getBox().h) {
					// if the tile is not a bridge the player will collide 
					if (tiles[i]->getType() != TILE_BRIDGE) {
						if (tiles[i]->getType() != TILE_MOVING_PLATFORM) {
							posRect.y = tiles[i]->getBox().y + tiles[i]->getBox().h;
							m_velY += -m_velY;
						}
					}
				}	
			}
		}
	}
	if (m_velY > 60) {
		bOnGround = false;
	}
		
}

void Player::Render(Sprite &playerTexture, SDL_Renderer* renderer, int camX, int camY)
{
	//SDL_QueryTexture(m_texture, NULL, NULL, &posRect.w, &posRect.h); 	
	playerTexture.render(renderer, posRect.x - camX, posRect.y - camY, &cropRect);
}

//setters and getters for velocity and collision box
float Player::GetVelX() {
	return m_velX;
}

float Player::GetVelY() {
	return m_velY;
}

SDL_Rect Player::GetBox() {
	return posRect;
}

void Player::Respawn() {
	posRect.x = startPosX;
	posRect.y = startPosY;
}

void Player::SetStartX(int x)
{
	startPosX = x;
}

void Player::SetStartY(int y)
{
	startPosY = y;
}


void Player::SetBoxX(int x)
{
	posRect.x = x;
}

void Player::SetBoxY(int y)
{
	posRect.y = y;
}
