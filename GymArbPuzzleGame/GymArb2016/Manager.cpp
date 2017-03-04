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
#include "Timer.h"
#include "WorldManager.h"

#include <SDL\SDL_thread.h>
Manager::Manager()
{
	//assigning variables
	window = nullptr;
	renderer = nullptr;

	//checks if the game is running	
	bIsRunning = true;

	//tells if the menu is shown
	showMenu = false;

	m_fullscreen = false;
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
	

	//Creates the timer object
	Timer timer;

	//Creates the menu object
	Menu menu(renderer, "menuBackground.png");

	world = new WorldManager(renderer);

	//creates the player object
	Player Player(renderer, world, playerTexture, 1, 1);

	//loads the media ( sprites etc. ) for the game. Loads the first level
	if (!loadMedia(world->tileSet, renderer, world->tileClips, "assets/levels/level1.map", world->playerStartX, world->playerStartY)) {
		std::cout << "Failed to load media!" << std::endl;
	}

	world->setTiles(world->tileSet, world->tileClips, "assets/levels/level1.map", world->playerStartX, world->playerStartY);

	//Loads the background music for the game
	Mix_Music *bgm = Mix_LoadMUS("assets/sounds/japanese_flute_music.mp3");

	//loads the background image for the game
	

	//creates the buttons for the menu
	menu.createButton(renderer, "Resume");
	menu.createButton(renderer, "Fullscreen");
	menu.createButton(renderer, "Exit To Desktop");

	menu.createMenu(); //sorts the buttons and places them in order

	std::string playerSpecsText;
	Sprite playerSpecs;
	SDL_Color textColor = { 0,0,0,0xFF };


	SDL_ShowCursor(0); //Hides the cursor

	
	//the game loop, runs as long as bIsRunning = true
	while (bIsRunning)
	{
		//Returns the current x and y coordinates of the mouse
		SDL_GetMouseState(&cursorTexture.xPos, &cursorTexture.yPos);

		//pauses music if menu is shown
		if (menu.getShowing()) {
			Mix_PauseMusic();
		}
		//else starts/resumes background music
		else if (menu.getShowing() == false) {
			if (!Mix_PlayingMusic()) {
				//Mix_PlayMusic(bgm, 0);
			}
			else if (Mix_PausedMusic()) {
				//Mix_ResumeMusic();
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
				if (menu.getShowing()) {
					if (menu.buttons[2].checkHover(evnt.button.x, evnt.button.y)) {
						bIsRunning = false;
					}
					if (menu.buttons[0].checkHover(evnt.button.x, evnt.button.y)) {
						menu.setShowing(false);
					}
					if (menu.buttons[1].checkHover(evnt.button.x, evnt.button.y)) {
						if (m_fullscreen) {
							SDL_SetWindowFullscreen(window, SDL_FALSE);
							m_fullscreen = false;
						}
						else {
							SDL_SetWindowFullscreen(window, SDL_TRUE);
							m_fullscreen = true;
						}
					}
				}
				break;

			case SDL_MOUSEMOTION:
				if (menu.getShowing())
					menu.checkHover(evnt.motion.x, evnt.motion.y);

			case SDL_KEYDOWN:
				switch (evnt.key.keysym.sym)
				{
					//sets showMenu = true/false when ESCAPE is pressed
				case SDLK_ESCAPE:
					if (!menu.getShowing()) {
						menu.setShowing(true);
					}
					else {
						menu.setShowing(false);
					}
					break;

				case SDLK_DOWN:
					if (menu.getShowing())
						menu.changeActive(1);
					break;
				case SDLK_UP:
					if (menu.getShowing())
						menu.changeActive(-1);
					break;

				case SDLK_RETURN:
					if (menu.getShowing())
						if (menu.buttons[0].getActive()) {
							bIsRunning = false;
						}
					if (menu.buttons[1].getActive()) {
						menu.setShowing(false);
					}
					if (menu.buttons[2].getActive()) {
						if (m_fullscreen) {
							SDL_SetWindowFullscreen(window, SDL_FALSE);
							m_fullscreen = false;
						}
						else {
							SDL_SetWindowFullscreen(window, SDL_TRUE);
							m_fullscreen = true;
						}
					}

				}
				break;
			}

			//takes in player input is menu is hidden
			if (menu.getShowing() == false)
				Player.ProcessInput(evnt, m_deltaTime); //takes the players input as an SDL_Event
		}


		//if nextLevel == true, the game loads the next level and sets nextLevel == false
		if (world->nextLevel) {
			world->loadNextLevel(Player);
		}

		//sets the default draw color
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		//clears the window
		SDL_RenderClear(renderer);

		//copies the background to the renderer
		SDL_RenderCopy(renderer, world->backgroundTexture, NULL, NULL); //copies the background to the renderer

																 //moves the player and camera if menu is hidden
		m_deltaTime = timer.getTicks() / 1000.f;
		
		if (!menu.getShowing()) {
			//renders the level
			for (int i = 0; i < world->TOTAL_TILES; ++i)
			{
				if (world->tileSet[i]->getType() == world->TILE_MOVING_PLATFORM) {
					world->tileSet[i]->movePlatform(world->tileSet, m_deltaTime);
				}
				world->tileSet[i]->render(tileTexture, renderer, world->tileClips); //renders the tiles to the renderer from the tile set vector
			}

			//movement function for the player, calculates the new position of the player and checks collision
			Player.Move(m_deltaTime, world->tileSet, world->nextLevel);
			//circle.move(tileSet, player);
			Player.Render(playerTexture, renderer); //renders the player


		}
		timer.start();

		//if showMenu == true, the menu is shown
		if (menu.getShowing())
			menu.showMenu(renderer);
		if (menu.getShowing())
			cursorTexture.render(renderer, cursorTexture.getX(), cursorTexture.getY());

		//presents the renderer
		SDL_RenderPresent(renderer); //prints out everything on the window

	}

	//closes down all systems and deallocates the tiles
	close(world->tileSet);

}


SDL_Texture *Manager::loadBackground(std::string path)
{
	
	//loads the background from file path
	SDL_Texture *background = IMG_LoadTexture(renderer, path.c_str());

	return background;
}


bool Manager::loadMedia(Tile* tiles[], SDL_Renderer * renderer, SDL_Rect tileClips[], std::string levelPath, int &playerX, int &playerY)
{
	//loading success flag
	bool success = true;

	//load tile texture
	if (!tileTexture.loadFromFile("assets/tiles/tileSpriteSheet.png", renderer)) {
		std::cout << "Failed to load tile set texture!" << std::endl;
		success = false;
	}

	if (!playerTexture.loadFromFile("assets/player/player.png", renderer)) {
		std::cout << "Failed to load player texture" << std::endl;
		success = false;
	}

	if (!circleTexture.loadFromFile("assets/misc/circle2.png", renderer)) {
		std::cout << "Failed to load circle texture" << std::endl;
		success = false;
	}


	if (!cursorTexture.loadFromFile("assets/cursor/cursor1.png", renderer)) {
		std::cout << "Failed to load cursor texture" << std::endl;
		success = false;
	}

	return success;
}
void Manager::close(Tile* tiles[])
{
	//SDL_DestroyTexture(backgroundTexture);
	//quits all initialized systems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
	Mix_Quit();


	//deallocate tiles
	for (int i = 0; i < world->TOTAL_TILES; i++)
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


float Manager::getHeight()
{
	return SCREEN_HEIGHT;
}

float Manager::getWidth()
{
	return SCREEN_WIDTH;
}
