#pragma once
#include <SDL\SDL.h>
#include <string>
#include "Sprite.h"
#include "Tile.h"

#include <String>


#include "Engine.h"

class Manager : public Engine
{
public:


	//Initializing variables
	Manager();

	//Calls for close() upon game exit
	~Manager();

	//Runs the game 
	void run();
	float getHeight();
	float getWidth();

	bool loadMedia(SDL_Renderer* renderer, std::string levelPath);

	void close(Tile* tiles[]);



	//Start the game loop
	void gameLoop();


	SDL_Event evnt;

	//scene textures
	Sprite tileTexture;

	Sprite buttonTexture;

	Sprite inputTextTexture;
	void loadNextLevel(Tile* tiles[], SDL_Rect tileClips[TOTAL_TILE_SPRITES]);

	static const int TOTAL_LEVELS = 5;
	std::string levels[TOTAL_LEVELS]{ "../../GymArbPuzzleGame/GymArb2016/assets/levels/level1.map", 
		"../../GymArbPuzzleGame/GymArb2016/assets/levels/level2.map" , 
		"../../GymArbPuzzleGame/GymArb2016/assets/levels/level3.map", 
		"../../GymArbPuzzleGame/GymArb2016/assets/levels/level4.map",
		"../../GymArbPuzzleGame/GymArb2016/assets/levels/level5.map" }; //All the levels
	int currentLevel; //Checks the current level


private:
	float m_frameTime = 0;
	float m_prevTime = 0;
	float m_currentTime = 0;
	float m_deltaTime = 0;

	bool m_fullscreen; //Flag for fullscreen

	bool nextLevel; //Flag for changing to the next level
	bool prevLevel; 

	bool showMenu; //Flag for showing menu

	bool canSearchBackgroundImage;
};

