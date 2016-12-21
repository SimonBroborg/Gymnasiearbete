#include "Manager.h"
#include <SDL\SDL_image.h>
#include <SDL\SDL_ttf.h>
#include <SDL\SDL_mixer.h>
#include <iostream>
#include "Player.h"
#include "Tile.h"
#include "Circle.h"
#include <fstream>
#include "Button.h"

Manager::Manager()
{	
	//assigning variables
	window = nullptr;
	renderer = nullptr;

	//checks if the game is running	
	bIsRunning = true;

	//checks if the next level should be loaded
	nextLevel = false;

	//the current level
	currentLevel = 0; 

	//tells if the menu is shown
	showMenu = false;
}



Manager::~Manager()
{
}

void Manager::run()
{
	//initializes the systems such as SDL, SDL_Image, creates window and renderer etc.
	initSystems();

	//starts the game loop
	gameLoop();
}

void Manager::gameLoop()
{
	//creates the player object
	Player player(renderer, playerTexture, 4, 4);

	//creates a circle object, this is currently a prototype
	Circle circle(renderer, 20, 90);

	//creates the array which will contain the map tiles
	Tile* tileSet[TOTAL_TILES];

	//array with clips from the tile sprite sheet
	SDL_Rect tileClips[TOTAL_TILE_SPRITES];

	//creates the camera object which makes the side-scrolling possible
	SDL_Rect camera = { 0,0, SCREEN_WIDTH, SCREEN_HEIGHT }; 
 
	//Creates the menu object
	Menu menu(renderer, "menuBackground.png");

	//loads the media ( sprites etc. ) for the game. Loads the first level
	if (!loadMedia(tileSet, renderer, tileClips, "assets/levels/level1.map", 0, 0)) {
		std::cout << "Failed to load media!" << std::endl;
	}

	//Loads the background music for the game
	Mix_Music *bgm = Mix_LoadMUS("assets/sounds/japanese_flute_music.mp3");

	//loads the background image for the game
	backgroundTexture = loadBackground("snowMountain.png");

	//creates the buttons for the menu
	menu.createButton(renderer, "assets/buttons/quit_game_button.png", SCREEN_HEIGHT / 2 - 44);
	menu.createButton(renderer, "assets/buttons/close_menu_button.png", SCREEN_HEIGHT / 2 + 50);

	
	//the game loop, runs as long as bIsRunning = true
 	while (bIsRunning)
	{
		//checks the delta time for each loop
		m_prevTime = m_currentTime;
		m_currentTime = SDL_GetTicks();
		m_deltaTime = (m_currentTime - m_prevTime) / 1000.0f;

		m_deltaTime = 0.03;
		
		//pauses music if menu is shown
		if (showMenu) {
			Mix_PauseMusic();
		}

		//else starts/resumes background music
		else if(!showMenu) {
			if (!Mix_PlayingMusic()) {
				Mix_PlayMusic(bgm, 0);
			}
			else if (Mix_PausedMusic()) {
				Mix_ResumeMusic();
			}
		}
		
		//checks for inputs from the player
		while (SDL_PollEvent(&evnt))
		{
			switch (evnt.type)
			{
			//if the 'x' button is pressed the game closes
			case SDL_QUIT:
				bIsRunning = false;
				break;

			//checks hover over buttons if user moves the mouse key
			case SDL_MOUSEBUTTONDOWN:
				if (showMenu) {
					if (menu.buttons[0].checkHover(evnt.button.x, evnt.button.y)) {
						bIsRunning = false;
					}
					if (menu.buttons[1].checkHover(evnt.button.x, evnt.button.y)) {
						showMenu = false;
					}
				}

				//places the player and the circle on the pressed coordinates
				circle.m_imgRect.x = evnt.button.x;
				circle.m_imgRect.y = evnt.button.y;
				player.posRect.x = evnt.button.x + camera.x;
				player.posRect.y = evnt.button.y + camera.y;

				break;

				//moves the circle when A, D and SPACE is pressed
			case SDL_KEYDOWN:
				switch (evnt.key.keysym.scancode)
				{
				case SDL_SCANCODE_A:
					circle.m_velX = -3;
					break;
				case SDL_SCANCODE_D:
					circle.m_velX = 3;
					break;

				case SDL_SCANCODE_SPACE:
					circle.m_velY = -4;
					break;

				//sets showMenu = true/false when ESCAPE is pressed
				case SDL_SCANCODE_ESCAPE:
					if (!showMenu) {
						showMenu = true;
					}
					else {
						showMenu = false;
					}
					break;
				}
				break;
			}
			
			//takes in player input is menu is hidden
			if(!showMenu)
				player.processInput(evnt, m_deltaTime); //takes the players input as an SDL_Event
		}

		//if nextLevel == true, the game loads the next level and sets nextLevel == false
		if (nextLevel == true) {
			currentLevel++;
			setTiles(tileSet, tileClips, levels[currentLevel].c_str(), 0, 0);	
			nextLevel = false;
		}
		
		//moves the player and camera if menu is hidden
		if (!showMenu) {
			//movement function for the player, calculates the new position of the player and checks collision
			player.move(m_deltaTime, tileSet, nextLevel); 	
			//circle.move(tileSet, player);

			camera.x = (player.posRect.x + player.posRect.w /2 ) - SCREEN_WIDTH / 2;
			camera.y = (player.posRect.y + player.posRect.h / 2) - SCREEN_HEIGHT / 2;

			//Keep the camera in bounds
			if (camera.x < 0)
			{
				camera.x = 0;
			}
			if (camera.y < 0)
			{
				camera.y = 0;
			}
			if (camera.x > LEVEL_WIDTH - camera.w)
			{
				camera.x = LEVEL_WIDTH - camera.w;
			}
			if (camera.y > LEVEL_HEIGHT - camera.h)
			{
				camera.y = LEVEL_HEIGHT - camera.h;
			}
		}

		//sets the default draw color
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		//clears the window
		SDL_RenderClear(renderer); 

		//copies the background to the renderer
		SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL); //copies the background to the renderer
		
		//renders the level
		for (int i = 0; i < TOTAL_TILES; i++)
		{
			if (tileSet[i]->getType() == TILE_MOVING_PLATFORM) {
				tileSet[i]->movePlatform(tileSet);
			}
			tileSet[i]->render(tileTexture, renderer, tileClips, camera); //renders the tiles to the renderer from the tile set vector
		}		

		
		//circle.render(circleTexture, renderer);
		player.render(playerTexture, renderer, camera.x, camera.y); //renders the player
		
		//if showMenu == true, the menu is shown
		if (showMenu)
			menu.showMenu(buttonTexture, renderer);
		
		//presents the renderer
		SDL_RenderPresent(renderer); //prints out everything on the window

	}

	//closes down all systems and deallocates the tiles
	close(tileSet);

}


SDL_Texture *Manager::loadBackground(std::string path)
{
	//loads the background from file path
	SDL_Texture *background = IMG_LoadTexture(renderer, path.c_str());

	return background;
}


bool Manager::loadMedia(Tile* tiles[], SDL_Renderer * renderer, SDL_Rect tileClips[], std::string levelPath, int playerX, int playerY)
{
	//loading success flag
	bool success = true;

	//load tile texture
	if (!tileTexture.loadFromFile("assets/tiles/tileSpriteSheet.png", renderer)) {
		std::cout << "Failed to load tile set texture!" << std::endl;
		success = false;
	}

	if (!playerTexture.loadFromFile("assets/player/doctorSS.png", renderer)) {
		std::cout << "Failed to load player texture" << std::endl;
		success = false;
	}

	if (!circleTexture.loadFromFile("assets/misc/circle2.png", renderer)) {
		std::cout << "Failed to load circle texture" << std::endl;
		success = false;
	}

	if (!buttonTexture.loadFromFile("assets/buttons/knapp.png", renderer)) {
		std::cout << "Failed to load button texture" << std::endl;
		success = false;
	}

	//Load tile map
	if (!setTiles(tiles, tileClips, levelPath.c_str(), playerX, playerY)) {
		std::cout << "Failed to load tile set ! " << std::endl;
		success = false;
	}

	return success;
}
void Manager::close(Tile* tiles[])
{
	SDL_DestroyTexture(backgroundTexture);
	//quits all initialized systems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
	Mix_Quit();

	
		//deallocate tiles
		for (int i = 0; i < TOTAL_TILES; i++)
		{
			if (tiles[i] == NULL) {
				delete tiles[i];
				tiles[i] = NULL;
			}
		}

		//free loaded images
		tileTexture.free();

		//destroy window
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		window = nullptr;
		renderer = nullptr;

		//quit SDL subsystems
		IMG_Quit();
		SDL_Quit();
}



bool Manager::setTiles(Tile* tiles[], SDL_Rect tileClips[], std::string levelPath, int playerX, int playerY)
{
	//success flag
	bool tilesLoaded = true;

	//get tile offsets
	int x = 0;
	int y = 0;

	//open the map
	std::ifstream map(levelPath.c_str());

	//if the map couldn't be loaded 
	if (!map.is_open()) {
		std::cout << "Unable to load map file!" << std::endl;
		tilesLoaded = false;
	}
	else {
		//initialize the tiles
		for (int i = 0; i < TOTAL_TILES; i++)
		{
			//determines what kind of tile will be made
			int tileType = -1;

			//red tile from map file
			map >> tileType;


			//if there was a problem reading the map
			if (map.fail()) {
				std::cout << "Error loading map! " << std::endl;
				tilesLoaded = false;
				break;
			}

			if ((tileType >= 0) && tileType < TOTAL_TILE_SPRITES) {
				tiles[i] = new Tile(x, y, tileType);
			}

			
			//if the don't recognize the tile type
			else {
				std::cout << "Error loading map! Invalid tile type" << std::endl;
				tilesLoaded = false;
				break;
			}

			//move to the next tile spot
			x += TILE_WIDTH;

			

			//if we have gone too far
			if (x >= LEVEL_WIDTH) {
				//Move back
				x = 0;

				//move to the next row
				y += TILE_HEIGHT;
			}
		}
		//clips the sprite sheet
		if (tilesLoaded) {
			tileClips[TILE_NONE].x = 0;
			tileClips[TILE_NONE].y = 0;
			tileClips[TILE_NONE].w = TILE_WIDTH;
			tileClips[TILE_NONE].h = TILE_HEIGHT;

			tileClips[TILE_BRIDGE].x = 0;
			tileClips[TILE_BRIDGE].y = 60;
			tileClips[TILE_BRIDGE].w = TILE_WIDTH;
			tileClips[TILE_BRIDGE].h = TILE_HEIGHT;

			tileClips[TILE_BOX].x = 0;
			tileClips[TILE_BOX].y = 120;
			tileClips[TILE_BOX].w = TILE_WIDTH;
			tileClips[TILE_BOX].h = TILE_HEIGHT;

			tileClips[TILE_GRASS_ROUNDED].x = 0;
			tileClips[TILE_GRASS_ROUNDED].y = 180;
			tileClips[TILE_GRASS_ROUNDED].w = TILE_WIDTH;
			tileClips[TILE_GRASS_ROUNDED].h = TILE_HEIGHT;

			tileClips[TILE_DIRT_CENTER ].x = 60;
			tileClips[TILE_DIRT_CENTER ].y = 180;
			tileClips[TILE_DIRT_CENTER ].w = TILE_WIDTH;
			tileClips[TILE_DIRT_CENTER ].h = TILE_HEIGHT;

			tileClips[TILE_GRASS_CLIFF_LEFT].x = 120;
			tileClips[TILE_GRASS_CLIFF_LEFT].y = 180;
			tileClips[TILE_GRASS_CLIFF_LEFT].w = TILE_WIDTH;
			tileClips[TILE_GRASS_CLIFF_LEFT].h = TILE_HEIGHT;

			tileClips[TILE_GRASS_CLIFF_RIGHT].x = 180;
			tileClips[TILE_GRASS_CLIFF_RIGHT].y = 180;
			tileClips[TILE_GRASS_CLIFF_RIGHT].w = TILE_WIDTH;
			tileClips[TILE_GRASS_CLIFF_RIGHT].h = TILE_HEIGHT;

			tileClips[TILE_PORTAL].x = 60;
			tileClips[TILE_PORTAL].y = 0;
			tileClips[TILE_PORTAL].w = TILE_WIDTH;
			tileClips[TILE_PORTAL].h = TILE_HEIGHT; 

			tileClips[TILE_GRASS_LEFT_ROUNDED].x = 240;
			tileClips[TILE_GRASS_LEFT_ROUNDED].y = 180;
			tileClips[TILE_GRASS_LEFT_ROUNDED].w = TILE_WIDTH;
			tileClips[TILE_GRASS_LEFT_ROUNDED].h = TILE_HEIGHT;

			tileClips[TILE_GRASS_CENTER].x = 300;
			tileClips[TILE_GRASS_CENTER].y = 180;
			tileClips[TILE_GRASS_CENTER].w = TILE_WIDTH;
			tileClips[TILE_GRASS_CENTER].h = TILE_HEIGHT;

			tileClips[TILE_GRASS_RIGHT_ROUNDED].x = 360;
			tileClips[TILE_GRASS_RIGHT_ROUNDED].y = 180;
			tileClips[TILE_GRASS_RIGHT_ROUNDED].w = TILE_WIDTH;
			tileClips[TILE_GRASS_RIGHT_ROUNDED].h = TILE_HEIGHT;

			tileClips[TILE_MOVING_PLATFORM].x = 121;
			tileClips[TILE_MOVING_PLATFORM].y = 0;
			tileClips[TILE_MOVING_PLATFORM].w = TILE_WIDTH;
			tileClips[TILE_MOVING_PLATFORM].h = TILE_HEIGHT;

			tileClips[TILE_MOVING_PLATFORM_STOP].x = 181;
			tileClips[TILE_MOVING_PLATFORM_STOP].y = 0;
			tileClips[TILE_MOVING_PLATFORM_STOP].w = TILE_WIDTH;
			tileClips[TILE_MOVING_PLATFORM_STOP].h = TILE_HEIGHT; 

			
		}
	}
	//Close the file
	map.close();


	return tilesLoaded;

}

float Manager::getHeight()
{
	return SCREEN_HEIGHT;
}

float Manager::getWidth()
{
	return SCREEN_WIDTH;
}

void Manager::loadNextLevel(Tile* tiles[], SDL_Rect tileClips[TOTAL_TILE_SPRITES]) {
	
	
}