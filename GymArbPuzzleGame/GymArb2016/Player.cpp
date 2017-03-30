//////////////////////////// ALL INCLUDED FILES ////////////////////////////////////////////////////////////
#include "Player.h"
#include <SDL/SDL_image.h>
#include <iostream>
#include <SDL/SDL_thread.h>
#include <stdio.h>
///////////////////////// CONSTRUCTOR /////////////////////////////////////////////////
Player::Player(SDL_Renderer* renderer, Sprite &playerTexture, float framesX, float framesY)
{
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
	m_maxHorVel = 250; //Maximum x velocity

	m_jumpHeight = 100;
	m_timeToJumpApex = .4f;

	//Calculates the gravity and jump power based on the wanted jump height and time for the jump
	m_gravity = m_jumpHeight * 2 / pow(m_timeToJumpApex, 2);
	m_jumpPower = sqrt(2 * m_gravity * m_jumpHeight);

	m_acceleration = 1.5;
	velYBeforeLand = 0;

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
	onSpring = false;
	isBouncing = false;

	//Sounds
	//playerJump = Mix_LoadWAV("assets/sounds/player_jump.wav");
	playerSaw = Mix_LoadWAV("assets/sounds/sawDeath.wav");

	/////////////////////// PREPARES FOR MOVEMENT ANIMATIONS ///////////////////////////////////////////////////////

	SDL_QueryTexture(m_texture, NULL, NULL, &cropRect.w, &cropRect.h); //cropRect's dimensions is equal to the sprite sheet
	m_textureWidth = cropRect.w; //Sets the texture width equal to the sprite sheet's width
	m_frameWidth = posRect.w = cropRect.w; // /= framesX; //Sets the width of each player frame
	m_frameHeight = posRect.h = cropRect.h;// /= framesY; //Sets the height of each player frame
}

////////////////////////////////////// DESSTRUCTOR /////////////////////////////////////////////////////////
Player::~Player()
{
}

///////////////////////////////////// READS INPUT FROM THE PLAYER ////////////////////////////////////////////////////

void Player::ProcessInput(SDL_Event &evnt, float delta)
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
				m_velY -= m_jumpPower + m_velY; //Sets the horizontal speed
				bOnGround = false; //The player is not on the ground
				bMoving = true; //The player is moving 
				Mix_PlayChannel(-1, playerJump, 0); //Plays jump sound
			}
			else if (bJumping == false && isBouncing == true) {
				bJumping = true; //The player is jumping
				m_velY -= m_jumpPower + m_velY; //Sets the horizontal speed
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
	static int platformSpeed = 0;

	///////////////////// NEW VALUES TO VARIABLES /////////////////////////////////////////
	m_xPos = posRect.x; //Saves horizontal position before movement
	m_yPos = posRect.y; //Saves vertical position before movement
	if (onMovingPlatform)
		posRect.x += platformSpeed * delta;

	posRect.x += m_velX * delta; //Changes horziontal position
	posRect.y += m_velY * delta; //Changes vertical position
	

	m_velY += m_gravity * delta; //Applies gravity to the vertical speed

	if (posRect.y > SCREEN_HEIGHT)
		Respawn();
	if (posRect.x <= 0)
		posRect.x = 0;
	if (posRect.x + posRect.w >= SCREEN_WIDTH)
		posRect.x = SCREEN_WIDTH - posRect.w;

	onSpring = false;
	////////////////////////////////// COLLISION CHECKING /////////////////////////////////////////////////////////
	for (int i = 0; i < TOTAL_TILES; ++i) { //Loops through all tiles
		if ((tiles[i]->getType() != TILE_NONE)) { //Checks collision if the tile type != 0 ( empty space ) 

				/////////////////////////// SAW COLLISION /////////////////////////////////////////////

			if (tiles[i]->getType() == TILE_SAW_1) { //If the tile is a saw
				if (circularCollision(tiles[i]->getBox(), posRect)) { //Checks circular collision with the saw
					Mix_PlayChannel(-1, playerSaw, 0); //Plays saw sound if there is a collision
					Respawn(); //Respawns the player
				}
			}
			////////////////////////// COLLISION WITH A TILE //////////////////////////////////////////////////////////7
			if (checkCollision(posRect, tiles[i]->getBox())) { //If there's a collision with the tile
															/////////////////////// PORTAL COLLISION /////////////////////////////
				if (tiles[i]->getType() == TILE_PORTAL) {
					nextLevel = true; //Changes to the next level
					delete tiles[i];
				}
				///////////////////// SPIKE COLLISION //////////////////////////////
				if (tiles[i]->getType() == TILE_SPIKES) {
					Respawn(); //The player dies and respawns
				}

				////////////////// 'NORMAL' TILE COLLISION /////////////////////////
				//checks if the player collides from the top of the tile
				if (m_yPos + posRect.h <= tiles[i]->getBox().y) {

					if (tiles[i]->getType() != TILE_MOVING_PLATFORM_STOP) {
						//if the tile is a bridge and "down" is pressed, the player will fall through
						if (tiles[i]->getType() == TILE_BRIDGE && bFallThrough == true)
						{
							m_velY += m_gravity * delta; //Falls through the tile
							bOnGround = false; //The player is not on the ground
							bJumping = true;
						}

						//stops the player from falling
						posRect.y = tiles[i]->getBox().y - posRect.h;
						bOnGround = true;
						bJumping = false;
						velYBeforeLand = m_velY; //Helps with the bouncing
						m_velY = 0;
						isBouncing = false;

						if (tiles[i]->getType() >= TILE_ICE_WHOLE && tiles[i]->getType() <= TILE_ICE_BROKEN_3) {
							tiles[i]->destroy(delta);
						}

						//if the tile is a moving platform, the player will get the same speed
						if (tiles[i]->getType() == TILE_MOVING_PLATFORM) {
							onMovingPlatform = true; //The player is on a moving platform
							platformSpeed = tiles[i]->getSpeed(); //Gets the platforms current velocity 
						}
						else {
							onMovingPlatform = false; //The player is not on a moving platform
						}

						//if the tile is not a bridge
						if (tiles[i]->getType() == TILE_SPRING) {
							onSpring = true; //The player is currently on a spring
							isBouncing = true; //The player will bounce
							bOnGround = false;
							bJumping = false;
							m_velY -= velYBeforeLand * 0.75; //The player "bounces" up with a slightly smaller velocity
						}
					}
				}

				//colliding from left
				else if (m_xPos - tiles[i]->getBox().w >= tiles[i]->getBox().x)
				{
					//Two if-statements checking if the tile is collidable from the left
					if (tiles[i]->getType() != TILE_BRIDGE && tiles[i]->getType() != TILE_MOVING_PLATFORM_STOP) {

						if (tiles[i]->getType() != TILE_MOVING_PLATFORM) {
							posRect.x = tiles[i]->getBox().x + tiles[i]->getBox().w;
						}
					}
				}

				// colliding from right
				else if (m_xPos + posRect.w <= tiles[i]->getBox().x)
				{
					//Two if-statements checking if the tile is collidable from the right
					if (tiles[i]->getType() != TILE_BRIDGE  && tiles[i]->getType() != TILE_MOVING_PLATFORM_STOP) {

						if (tiles[i]->getType() != TILE_MOVING_PLATFORM) {
							posRect.x = tiles[i]->getBox().x - posRect.w;
						}
					}
				}

				//colliding from the bottom
				else if (m_yPos >= tiles[i]->getBox().y + tiles[i]->getBox().h) {

					// if the tile is not a bridge the player will collide 
					//Two if-statements checking the the tile is collidable from the bottom 
					if (tiles[i]->getType() != TILE_BRIDGE  && tiles[i]->getType() != TILE_MOVING_PLATFORM_STOP) {

						if (tiles[i]->getType() != TILE_MOVING_PLATFORM) {
							posRect.y = tiles[i]->getBox().y + tiles[i]->getBox().h;
							m_velY += -m_velY; //Reverses the y velocity, making the player "bounce"
						}
					}
				}

			}
		}
	}

	//Quick fix to check if the player is falling
	if (m_velY > 80) {
		bOnGround = false;
	}
}

//Renders the player to the renderer
void Player::Render(Sprite &playerTexture, SDL_Renderer* renderer)
{
	//SDL_QueryTexture(m_texture, NULL, NULL, &posRect.w, &posRect.h); 	
	playerTexture.render(renderer, posRect.x, posRect.y, &cropRect);
}

//If the player respawns
void Player::Respawn() {
	SDL_Delay(500);
	//Resets the players velocity
	m_velY = 0;
	//Resets the players position
	posRect.x = startPosX;
	posRect.y = startPosY;
}

//Getters for the player
float Player::GetVelX() {
	return m_velX;
}
float Player::GetVelY() {
	return m_velY;
}
SDL_Rect Player::GetBox() {
	return posRect;
}

//Setters for the player
void Player::SetStartX(int x){
	startPosX = x; //Sets the start x position
}
void Player::SetStartY(int y){
	startPosY = y; //Sets the start y position
}
void Player::SetBoxX(int x){
	posRect.x = x; //Sets the x position
}
void Player::SetBoxY(int y){
	posRect.y = y; //Sets the y position
}
