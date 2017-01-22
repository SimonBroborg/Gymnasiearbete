#include "Manager.h"
#include <SDL\SDL_image.h>
#include <SDL\SDL_ttf.h>
#include <SDL\SDL_mixer.h>
#include <iostream>
#include "Tile.h"
#include <fstream>
#include "Menu.h"
#include "LevelCreator.h"


Manager::Manager()
{
	//assigning variables
	window = nullptr;
	renderer = nullptr;

	//checks if the game is running	
	bIsRunning = true;

	//checks if the next level should be loaded
	nextLevel = false;
	prevLevel = false;

	//the current level
	currentLevel = 0;

	//tells if the menu is shown
	showMenu = false;

	m_fullscreen = false;

	canSearchBackgroundImage = false;
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
	Menu menu(renderer, "menuBackground.png");

	LevelCreator creator;
	//creates the camera object which makes the side-scrolling possible
	SDL_Rect camera = { 0,0, SCREEN_WIDTH, SCREEN_HEIGHT };


	//loads the media ( sprites etc. ) for the game. Loads the first level
	if (!loadMedia(renderer,"")) {
		std::cout << "Failed to load media!" << std::endl;
	}

	creator.setTiles(levels[currentLevel]);
	//Loads the background music for the game

	//loads the background image for the game
	backgroundTexture = loadBackground("");

	//creates the buttons for the menu
	menu.createButton(renderer, "assets/buttons/quit_game_button.png", SCREEN_HEIGHT / 2 - 44);
	menu.createButton(renderer, "assets/buttons/change_background_button.png", SCREEN_HEIGHT / 2 + 50);
	menu.createButton(renderer, "assets/buttons/save_level_button.png", SCREEN_HEIGHT / 2 + 150);

	std::string text = "Enter file path";
	SDL_Color textColor = { 0,0,0,0xFF };

	bool renderText;

	inputTextTexture.loadFromRenderedText(text, textColor, renderer);
	SDL_StartTextInput();
	//the game loop, runs as long as bIsRunning = true
	while (bIsRunning)
	{
		renderText = false;

		//checks the delta time for each loop

		//pauses music if menu is shown
		if (showMenu) {
			Mix_PauseMusic();
		}
		//else starts/resumes background music
		else if (!showMenu) {
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
				if (showMenu) {
					if (menu.buttons[0].checkHover(evnt.button.x, evnt.button.y)) {
						bIsRunning = false;
					}
					if (menu.buttons[1].checkHover(evnt.button.x, evnt.button.y)) {
						menu.buttons[1].setImage(renderer, "assets/buttons/change_background_button_transparent.png");
						text = "Enter file path";
						canSearchBackgroundImage = true;
					}

					if (menu.buttons[2].checkHover(evnt.button.x, evnt.button.y)) {
						creator.saveTiles(levels[currentLevel]); //Saves the current level
						showMenu = false; 
					}

				}
				if (!showMenu)
					creator.putTile(evnt.button.x, evnt.button.y); //Puts current tile type onto the hovered tile spot

				break;


			case SDL_KEYDOWN:
				switch (evnt.key.keysym.sym)
				{
					//sets showMenu = true/false when ESCAPE is pressed
				case SDLK_ESCAPE:
					if (!showMenu) {
						showMenu = true;
					}
					else {
						showMenu = false;
					}
					break;
				case SDLK_BACKSPACE:
					if (text.length() > 0) {
						text.pop_back();
						renderText = true;
					}
					break;

				case SDLK_UP:
					creator.currentType--;
					if (creator.currentType < 0)
						creator.currentType = TOTAL_TILE_SPRITES - 1;
					delete creator.tileSet[0];
					creator.tileSet[0] = new Tile(0, 0, creator.currentType);

					break;

				case SDLK_DOWN:
					creator.currentType++;
					if (creator.currentType > TOTAL_TILE_SPRITES -  2)
						creator.currentType = 0;
					delete creator.tileSet[0];
					creator.tileSet[0] = new Tile(0, 0, creator.currentType);

					break;

				case SDLK_LEFT:
					prevLevel = true;
					break;

				case SDLK_RIGHT:
					nextLevel = true;
					break;

				case SDLK_RETURN:
					if (canSearchBackgroundImage) {
						menu.buttons[1].setImage(renderer, "assets/buttons/change_background_button.png");
						canSearchBackgroundImage = false;
						backgroundTexture = loadBackground(text);
						text = "";
						renderText = true;
						showMenu = false; 
					}
					break;

				case SDLK_DELETE:
					for (int i = 0; i < TOTAL_TILES; i++) {
						creator.tileSet[i]->setType(TILE_NONE);
					}
				}
				break;

			case SDL_TEXTINPUT:
				if (canSearchBackgroundImage) {
					text += evnt.text.text;
					renderText = true;
				}
				break;
				break;

			}

		}

		//if nextLevel == true, the game loads the next level and sets nextLevel == false
		if (nextLevel) {
			currentLevel++;
			if (currentLevel >= TOTAL_LEVELS)
				currentLevel = 0;
			creator.setTiles(levels[currentLevel].c_str());
			nextLevel = false;
		}
		if (prevLevel) {
			currentLevel--;
			if (currentLevel < 0)
				currentLevel = TOTAL_LEVELS - 1;
			creator.setTiles(levels[currentLevel].c_str());
			prevLevel = false; 
		}
		m_prevTime = m_currentTime;
		m_currentTime = SDL_GetTicks();
		//moves the player and camera if menu is hidden
		if (!showMenu) {
			//movement function for the player, calculates the new position of the player and checks collision

			//circle.move(tileSet, player);
			m_deltaTime = (m_currentTime - m_prevTime) / 1000.0f;

		}

		//sets the default draw color
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		//clears the window
		SDL_RenderClear(renderer);

		//copies the background to the renderer
		SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL); //copies the background to the renderer



		if (renderText) {
			//Text is not empty
			if (text != "")
			{
				//Render new text
				inputTextTexture.loadFromRenderedText(text.c_str(), textColor, renderer);
			}
			//Text is empty
			else
			{
				//Render space texture
				inputTextTexture.loadFromRenderedText(" ", textColor, renderer);
			}
		}

		//renders the level
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			if (creator.tileSet[i]->getType() == TILE_MOVING_PLATFORM) {
				creator.tileSet[i]->movePlatform(creator.tileSet);
			}
			creator.tileSet[i]->render(tileTexture, renderer, creator.tileClips); //renders the tiles to the renderer from the tile set vector
		}

		//if showMenu == true, the menu is shown
		if (showMenu)
			menu.showMenu(buttonTexture, renderer);

		if (canSearchBackgroundImage && showMenu)
			inputTextTexture.render(renderer, menu.buttons[1].getBox().x + menu.buttons[1].getBox().w / 2 - inputTextTexture.getWidth() / 2, menu.buttons[1].getBox().y + menu.buttons[1].getBox().h / 2 - inputTextTexture.getHeight() / 2);

		//presents the renderer
		SDL_RenderPresent(renderer); //prints out everything on the window

	}

	SDL_StopTextInput(); //Stops text input

	//closes down all systems and deallocates the tiles
	close(creator.tileSet);

}


SDL_Texture *Manager::loadBackground(std::string path)
{
	//loads the background from file path
	SDL_Texture *background = IMG_LoadTexture(renderer, path.c_str());

	return background;
}


bool Manager::loadMedia(SDL_Renderer * renderer, std::string levelPath)
{
	//loading success flag
	bool success = true;

	//load tile texture
	if (!tileTexture.loadFromFile("assets/tiles/tileSpriteSheet.png", renderer)) {
		std::cout << "Failed to load tile set texture!" << std::endl;
		success = false;
	}


	if (!buttonTexture.loadFromFile("assets/buttons/knapp.png", renderer)) {
		std::cout << "Failed to load button texture" << std::endl;
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

