#include "Player.h"
#include <SDL/SDL_image.h>
#include <iostream>

Player::Player(SDL_Renderer *renderer, std::string filePath, float x, float y, float framesX, float framesY, float windowWidth, float windowHeight)
{
	SDL_Surface *surface = IMG_Load(filePath.c_str());
	if (surface == nullptr)
		std::cout << "Error: " << SDL_GetError() << std::endl;
	
	else
	{
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture == nullptr)
			std::cout << "Error: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(surface);

	SDL_QueryTexture(texture, nullptr, nullptr, &cropRect.w, &cropRect.h);

	//Displays the player on chosen coordinates
	posRect.x = x;
	posRect.y = y;

	//sets the width of the cropRect which will display the different frames of the movement animation
	m_textureWidth = cropRect.w;
	
	//sets the width of each frame
	cropRect.w /= framesX;

	//set the height of each frame
	cropRect.h /= framesY;

	//Set the width and height of the frames and rects equal to each other
	m_frameWidth = posRect.w = cropRect.w;
	m_frameHeight = posRect.h = cropRect.h = 30;

	m_leftSpeed = -180.0f * 0.7; //players velocity to the left
	m_rightSpeed = 180.0f * 0.7; //players velocity to the right
	m_jumpSpeed = 200.0f; //the speed or power that the player jumps with
	m_gravity = 400.0f; //gravity which pushes the player downwards

	bJumping = false; //The player is not jumping when the game starts
	bMoving = false; //The player is not moving when the game starts
	bFallThrough = false; //checks if it is possible to fall through block

	m_windowHeight = windowHeight;
	m_windowWidth = windowWidth;

	m_jumpHeight = 0;
}


Player::~Player()
{
	SDL_DestroyTexture(texture);
}


void Player::render(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, texture, &cropRect, &posRect);
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

void Player::move(float delta, std::vector<SDL_Rect> rects)
{
		//the position of the player before any movement calculations
		m_xPos = posRect.x;
		m_yPos = posRect.y;

		
		//moves the player's x coordinate equal to the x velocity times delta
		posRect.x += m_velX * delta;

		//moves the player equal to the velocity
		posRect.y += m_velY * delta;

		//changes the velocity so slowly fall down again
		m_velY += m_gravity * delta;
		

		
		//Makes sure the player stays inside the games borders, and falls through the bottom
		keepInsideBorder();

		//collision checking for each object inside the vectors "rects"
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
			std::cout << m_yPos << std::endl;

		
}

//returns true if there is a collision, otherwise it returns false
bool Player::checkCollision(const SDL_Rect& posRect, const SDL_Rect& obj)
{
	//sets up variables which keeps track of the different parts of the rect
	//which makes it easier to understand the collision check 
	//posRect is the players coordinates, height and width, and obj for the object the player collides with
	float playerLeft, objLeft;
	float playerRight, objRight;
	float playerTop, objTop;
	float playerBottom, objBottom;

	playerLeft = posRect.x;
	playerRight = posRect.x + posRect.w;
	playerTop = posRect.y;
	playerBottom = posRect.y + posRect.h;

	objLeft = obj.x;
	objRight = obj.x + obj.w;
	objTop = obj.y;
	objBottom = obj.y + obj.h;

	//if none of the proceeding if-statements is true, there is a collision
	if (playerBottom <= objTop)
		return false;

	if (playerTop >= objBottom)
		return false;

	if (playerRight <= objLeft)
		return false;

	if (playerLeft >= objRight)
		return false;

	return true;
}

//Makes sure the player cant go outside the window
void Player::keepInsideBorder()
{
	//makes sure the player cant go outside the window on the left
	if (posRect.x < 0)
		posRect.x = 0;

	//makes sure the player cant go outside the window on the right
	if (posRect.x + posRect.w >= m_windowWidth)
		posRect.x = m_windowWidth - posRect.w;

	//the player cant fall through the bottom of the window
	if (posRect.y + posRect.h > m_windowHeight)
	{
		posRect.y = m_windowHeight - posRect.h;
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