#include "Manager.h"
#include <SDL\SDL_image.h>
#include <SDL\SDL_ttf.h>
#include <SDL\SDL_mixer.h>
#include <iostream>
#include "Player.h"
#include "Tile.h"
#include "Circle.h"
#include <fstream>

Manager::Manager()
{
	window = nullptr;
	renderer = nullptr;
	bIsRunning = true;

}


Manager::~Manager()
{
}

void Manager::run()
{
	initSystems();
	gameLoop();
}

void Manager::gameLoop()
{
	//Player player(renderer, 1, 1);
	Tile* tileSet[TOTAL_TILES];

	if (!loadMedia(tileSet, renderer)) {
		std::cout << "Failed to load media!" << std::endl;
	}

	SDL_Rect camera = { 0,0, SCREEN_WIDTH, SCREEN_HEIGHT };

	

	Mix_Music *bgm = Mix_LoadMUS("backgroundMusic.mp3");

	backgroundTexture = loadBackground("kjellsbg.png");


	//Circle circle(renderer, 70, 90);

	//circle.loadSprite(renderer, "circle2.png", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);


	//player.loadSprite(renderer, "gameCharacter.png", 0, SCREEN_HEIGHT / 2);
	//player.setUp();
	std::cout << tileClips[2].x << " " << tileClips[2].y << " " << tileClips[2].h;
	while (bIsRunning)
	{
		m_prevTime = m_currentTime;
		m_currentTime = SDL_GetTicks();
		m_deltaTime = (m_currentTime - m_prevTime) / 1000.0f;

		if (m_deltaTime < 0.03f)
			m_deltaTime = 0.03f;

		//plays the background music when the game starts
		if (!Mix_PlayingMusic()) {
			//Mix_PlayMusic(bgm, 0);
		}

		while (SDL_PollEvent(&evnt))
		{
			switch (evnt.type)
			{
			case SDL_QUIT:
				bIsRunning = false;
				break;

			case SDL_MOUSEBUTTONDOWN:
				/*circle.xPos = evnt.button.x;
				circle.yPos = evnt.button.y;*/
				//player.posRect.x = evnt.button.x;
				//player.posRect.y = evnt.button.x;
				break;

			case SDL_KEYDOWN:
				switch (evnt.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
					bIsRunning = false;
					break;
				case SDL_SCANCODE_A:
					//circle.m_velX = -2;
					break;
				case SDL_SCANCODE_D:
					//circle.m_velX = 2;
					break;

				case SDL_SCANCODE_SPACE:
					//circle.m_velY = -6;
					break;
				}
				break;


			}
			//player.processInput(evnt, m_deltaTime); //takes the players input as an SDL_Event

		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer); //clears the window

		
		SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL); //copies the background to the renderer

		//player.move(m_deltaTime, rects); //movement function for the player, calculates the new position of the player and checks collision

		//circle.move(rects, player.posRect, player.getVelX() * m_deltaTime);

		//rects.clear(); //Clears the vector "rects" to avoid lag caused by to much memory use



 		//render level
 		for (int i = 0; i < TOTAL_TILES; i++)
 		{
			tileSet[i]->render(camera, tileTexture, renderer); //renders the tiles to the renderer from the tile set vector
		}


		//circle.draw();
		//player.draw(); //renders the player
		SDL_RenderPresent(renderer); //prints out everything on the window
	}
	close(tileSet);

}


SDL_Texture *Manager::loadBackground(std::string path)
{
	SDL_Texture *background = IMG_LoadTexture(renderer, path.c_str());

	return background;
}




bool Manager::loadMedia(Tile* tiles[], SDL_Renderer * renderer)
{
	//loading success flag
	bool success = true;

	//load tile texture
	if (!tileTexture.loadFromFile("tiles.png", renderer)) {
		std::cout << "Failed to load tile set texture!" << std::endl;
		success = false;
	}

	//Load tile map
	if (!setTiles(tiles)) {
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



bool Manager::setTiles(Tile* tiles[])
{
	//success flag
	bool tilesLoaded = true;

	//get tile offsets
	int x = 0;
	int y = 0;

	//open the map
	std::ifstream map("lazy.map");

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
		std::cout << tilesLoaded << std::endl;
		//clips the sprite sheet
		if (tilesLoaded) {
			tileClips[TILE_RED].x = 0;
			tileClips[TILE_RED].y = 0;
			tileClips[TILE_RED].w = TILE_WIDTH;
			tileClips[TILE_RED].h = TILE_HEIGHT;

			tileClips[TILE_GREEN].x = 0;
			tileClips[TILE_GREEN].y = 80;
			tileClips[TILE_GREEN].w = TILE_WIDTH;
			tileClips[TILE_GREEN].h = TILE_HEIGHT;

			tileClips[TILE_BLUE].x = 0;
			tileClips[TILE_BLUE].y = 160;
			tileClips[TILE_BLUE].w = TILE_WIDTH;
			tileClips[TILE_BLUE].h = TILE_HEIGHT;
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