#pragma once
/////////////////////////////////////// ALL INCLUDED FILES ///////////////////////////////////////////////////////////////
#include <SDL\SDL.h>
#include <string>
#include <vector>
#include "Sprite.h"
#include <SDL/SDL_image.h>
#include <iostream>
#include <SDL/SDL_thread.h>
#include <stdio.h>


#include "Entity.h"



class WorldManager;
class Tile;
/////////////////////////////////////////////////// THE Player CLASS, INHERITS FROM Engine //////////////////////////////////////////////////////////
class Player : public  Entity
{
//////////////////////////////////////////// PUBLIC ////////////////////////////////////////////////////////////////////
public:
	//Constructor, parameter for renderer, Sprite and the number of frames on x and y
	Player(SDL_Renderer* renderer, WorldManager *world, Sprite &playerTexture, float framesX, float framesY);
	//Destructor
	~Player();

//////////////////////////////// PUBLIC FUNCTIONS //////////////////////////////////////////////////////////////

	void ProcessInput(SDL_Event &evnt, float delta); //Reads input from player
	void Move(float delta, Tile* tiles[], bool &nextLevel); //Moves the player + checks tile collision
	void Render(Sprite &playerTexture, SDL_Renderer* renderer); //Renders the player	
	
	//Getters
	float GetVelX(); //gets horizontal velocity
	float GetVelY(); //gets vertical velocity
	SDL_Rect GetBox(); //gets the collision box

	//Setters
	void SetStartX(int x); //Set the horizontal start position
	void SetStartY(int y); //Sets the vertical start position
	void SetBoxX(int x); //Sets the horizontal position
	void SetBoxY(int y); //Sets the vertical position

	void Respawn(); //Moves the player to start positions

	//bools
	bool bJumping; //Checks if the player is jumping
	bool bMoving; //Checks if the player is moving
	bool bOnGround; //Checks if the player is on ground
	bool bFallThrough; //Checks if player want to fall through a tile
	bool bSprint;
	bool onMovingPlatform;
	bool onSpring; 
	bool isBouncing; 
	bool isFalling; //Checks if the player is falling

/////////////////////////////////////// PRIVATE /////////////////////////////////////////////////////////////////////
private:

////////////////////////////////// PLAYER VALUES /////////////////////////////////////////////////////

	//SDL
	SDL_Rect posRect; //Collision box + keeps track of position

	//ints
	int startPosX; //Horizontal start position
	int startPosY; //Vertical start position
	int m_jumpTime; 
	int velYBeforeLand;
	int velXBeforeColl;
	
	//floats
	float m_velX; //Horizontal velocity
	float m_velY; //Vertical velocity
	float m_gravity; //the players velocity
	float m_acceleration; 
	float m_jumpHeight;
	float m_timeToJumpApex; 

	float m_xPos; //Horizontal position before movement, helps with collision checking
	float m_yPos; //Vertical position before movement, helps with collision checking

	//doubles
	double m_maxHorVel; //The maximum horizontal velocity
	double m_jumpPower; //The speed of the jump
	

////////////////////////////////////////// PLAYER SOUNDS ///////////////////////////////////////////////

	Mix_Chunk *playerJump; //Jump sound
	Mix_Chunk *playerSaw; //Sound when moving into a saw

//////////////////////////////////// PRIVATE FUNCTIONS /////////////////////////////////////////////////
	

///////////////////////// PLAYER TEXTURE AND ANIMATIONS //////////////////////////////////////////////////

	//SDL
	SDL_Texture* m_texture; //Sprite sheet
	SDL_Rect cropRect; //Shows current frame from the sprite sheet

	//Floats
	float m_frameCounter; //Keeps track of the current frame
	float m_frameWidth; //Width of the frame (the player's width)
	float m_frameHeight; //Height of the frame (the player's height
	float m_textureWidth; //Width of the sprite sheet

};

