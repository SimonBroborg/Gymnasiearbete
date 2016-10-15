#pragma once
#include <SDL\SDL.h>
#include <string>
#include <vector>
class Player
{
public:
	//creates the player sprite, x and y position and the number of frames for the player when the game starts
	Player(SDL_Renderer *renderer, std::string filePath, int x, int y, int framesX, int framesY, int windowWidth, int windowHeight);
	~Player();

	//Renders the player texture based on the position
	void render(SDL_Renderer *renderer);

	//takes the players inputs and calculates the movement of the player
	void processInput(SDL_Event &evnt, float delta);

	//moves the player based on the posRect's x and y coordinates. Also calls the checkCollision() function
	void move(float delta, std::vector<SDL_Rect> rects);

	void keepInsideBorder();

private:
	//checks the collision between the player and another rect. Returns true if there is a collision.
	bool checkCollision(const SDL_Rect& posRect, const SDL_Rect& obj);

	//The player sprite
	SDL_Texture *texture;

	//Booleans which checks movement, jumping and if the player is on the ground
	bool bJumping, bMoving, bOnGround, bFallThrough;

	//the x and y velocity
	float m_velX, m_velY;

	//the speed which the player moves at to the left and right
	double m_leftSpeed, m_rightSpeed, m_jumpSpeed;

	//the players gravity
	int m_gravity;

	//Keeps track of the players position
	SDL_Rect posRect;

	//Shows the current frame of the player on the sprite sheet
	SDL_Rect cropRect;

	//Checks which the active frame is
	float m_frameCounter;

	//the dimensions of the player. Will help chosing the right part of the players sprite sheet
	int m_frameWidth, m_frameHeight;

	//width of the whole texture
	int m_textureWidth;

	int m_acceleration;

	//keeps track of the x and y coords before any movement. Used to know from which side the collision is from. 
	float m_xPos, m_yPos;

	int m_windowWidth, m_windowHeight;

	int m_jumpHeight;
};

