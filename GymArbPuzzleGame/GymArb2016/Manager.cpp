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
	//creates the player object
	Player Player(renderer, playerTexture, 1, 1);
	int playerStartX, playerStartY;

	//Creates the timer object
	Timer timer;



	//creates the camera object which makes the side-scrolling possible
	SDL_Rect camera = { 0,0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//Creates the menu object
	Menu menu(renderer, "menuBackground.png");



	WorldManager world;

	//loads the media ( sprites etc. ) for the game. Loads the first level
	if (!loadMedia(world.tileSet, renderer, world.tileClips, "assets/levels/level1.map", playerStartX, playerStartY)) {
		std::cout << "Failed to load media!" << std::endl;
	}

	world.setTiles(world.tileSet, world.tileClips, "assets/levels/level1.map", playerStartX, playerStartY);

	//Loads the background music for the game
	Mix_Music *bgm = Mix_LoadMUS("assets/sounds/japanese_flute_music.mp3");

	//loads the background image for the game
	backgroundTexture = loadBackground("assets/backgrounds/grayBackground.jpg");

	//creates the buttons for the menu
	menu.createButton(renderer, "assets/buttons/quit_game_button.png");
	menu.createButton(renderer, "assets/buttons/close_menu_button.png");
	menu.createButton(renderer, "assets/buttons/fullscreen_button.png");
	

	menu.createMenu();

	std::string playerSpecsText;
	Sprite playerSpecs;
	SDL_Color textColor = { 0,0,0,0xFF };


	SDL_ShowCursor(0);

	//the game loop, runs as long as bIsRunning = true
	while (bIsRunning)
	{
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
					if (menu.buttons[0].checkHover(evnt.button.x, evnt.button.y)) {
						bIsRunning = false;
					}
					if (menu.buttons[1].checkHover(evnt.button.x, evnt.button.y)) {
						menu.setShowing(false);
					}
					if (menu.buttons[2].checkHover(evnt.button.x, evnt.button.y)) {
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


				//places the player and the circle on the pressed coordinates

				Player.SetBoxX(evnt.button.x);
				Player.SetBoxY(evnt.button.y);
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
					if(menu.getShowing())
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
				Player.Update(evnt, m_deltaTime); //takes the players input as an SDL_Event
		}





		//if nextLevel == true, the game loads the next level and sets nextLevel == false
		if (nextLevel == true) {
			currentLevel++;
			world.setTiles(world.tileSet, world.tileClips, levels[currentLevel].c_str(), playerStartX, playerStartY);
			nextLevel = false;
			Player.SetStartX(playerStartX);
			Player.SetStartY(playerStartY);
			Player.Respawn();
		}

		m_deltaTime = timer.getTicks() / 1000.f;
		//moves the player and camera if menu is hidden
		if (menu.getShowing() == false) {
			//movement function for the player, calculates the new position of the player and checks collision
			Player.Move(m_deltaTime, world.tileSet, nextLevel);
			//circle.move(tileSet, player);
			timer.start();
			camera.x = (Player.GetBox().x + Player.GetBox().w / 2) - SCREEN_WIDTH / 2;
			camera.y = (Player.GetBox().y + Player.GetBox().h / 2) - SCREEN_HEIGHT / 2;

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
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			if (world.tileSet[i]->getType() == TILE_MOVING_PLATFORM) {
				world.tileSet[i]->movePlatform(world.tileSet);
			}
			world.tileSet[i]->render(tileTexture, renderer, world.tileClips); //renders the tiles to the renderer from the tile set vector
		}


		Player.Render(playerTexture, renderer, camera.x, camera.y); //renders the player

		//if showMenu == true, the menu is shown
		if (menu.getShowing())
			menu.showMenu(buttonTexture, renderer);

		playerSpecsText = "Player x: " + std::to_string(Player.GetBox().x) +
			"   " + "Player y: " + std::to_string(Player.GetBox().y) +
			"   " + "Player vel x: " + std::to_string(Player.GetVelX()) +
			"   " + "Player vel y: " + std::to_string(Player.GetVelY()) +
			"   " + "Jumping: " + std::to_string(Player.bJumping) +
			"   " + "On ground: " + std::to_string(Player.bOnGround);
		//playerSpecs.loadFromRenderedText(playerSpecsText.c_str(), textColor, renderer);
		playerSpecs.render(renderer, 100, 100);

		if (menu.getShowing())
			cursorTexture.render(renderer, cursorTexture.getX(), cursorTexture.getY());

		//presents the renderer
		SDL_RenderPresent(renderer); //prints out everything on the window

	}

	//closes down all systems and deallocates the tiles
	close(world.tileSet);

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

	if (!buttonTexture.loadFromFile("assets/buttons/knapp.png", renderer)) {
		std::cout << "Failed to load button texture" << std::endl;
		success = false;
	}
	if (!buttonTexture.loadFromFile("assets/buttons/knapp.png", renderer)) {
		std::cout << "Failed to load button texture" << std::endl;
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