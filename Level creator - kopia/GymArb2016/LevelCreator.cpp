#include "LevelCreator.h"
#include <fstream>
#include <iostream>

LevelCreator::LevelCreator()
{
}


LevelCreator::~LevelCreator()
{
}


bool LevelCreator::setTiles(std::string levelPath)
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
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			//determines what kind of tile will be made
			int tileType = -1;

			//read tile from map file
			map >> tileType;


			//if there was a problem reading the map
			if (map.fail()) {
				std::cout << "Error loading map! " << std::endl;
				tilesLoaded = false;
				break;
			}

			if (tileType == TILE_SAW_1) {
				x -= TILE_WIDTH / 2;
				y -= TILE_HEIGHT / 2;
				tileSet[i] = new Tile(x, y, tileType);
				x += TILE_WIDTH / 2;
				y += TILE_HEIGHT / 2;
			}


			else if ((tileType >= 0) && tileType < TOTAL_TILE_SPRITES) {
				tileSet[i] = new Tile(x, y, tileType);

			}

			//if the til type is not recognized
			else {
				std::cout << "Error loading map! Invalid tile type" << std::endl;
				tilesLoaded = false;
				break;
			}

			//move to the next tile spot horizontally
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
			tileClips[TILE_NONE].w = 0;
			tileClips[TILE_NONE].h = 0;

			tileClips[TILE_BRIDGE].x = 0;
			tileClips[TILE_BRIDGE].y = TILE_HEIGHT;
			tileClips[TILE_BRIDGE].w = TILE_WIDTH;
			tileClips[TILE_BRIDGE].h = TILE_HEIGHT;

			tileClips[TILE_BOX].x = 0;
			tileClips[TILE_BOX].y = TILE_HEIGHT * 2;
			tileClips[TILE_BOX].w = TILE_WIDTH;
			tileClips[TILE_BOX].h = TILE_HEIGHT;

			tileClips[TILE_GRASS_ROUNDED].x = 0;
			tileClips[TILE_GRASS_ROUNDED].y = TILE_HEIGHT * 3;
			tileClips[TILE_GRASS_ROUNDED].w = TILE_WIDTH;
			tileClips[TILE_GRASS_ROUNDED].h = TILE_HEIGHT;

			tileClips[TILE_DIRT_CENTER].x = TILE_WIDTH;
			tileClips[TILE_DIRT_CENTER].y = TILE_HEIGHT * 3;
			tileClips[TILE_DIRT_CENTER].w = TILE_WIDTH;
			tileClips[TILE_DIRT_CENTER].h = TILE_HEIGHT;

			tileClips[TILE_GRASS_CLIFF_LEFT].x = TILE_WIDTH * 2;
			tileClips[TILE_GRASS_CLIFF_LEFT].y = TILE_HEIGHT * 3;
			tileClips[TILE_GRASS_CLIFF_LEFT].w = TILE_WIDTH;
			tileClips[TILE_GRASS_CLIFF_LEFT].h = TILE_HEIGHT;

			tileClips[TILE_GRASS_CLIFF_RIGHT].x = TILE_WIDTH * 3;
			tileClips[TILE_GRASS_CLIFF_RIGHT].y = TILE_HEIGHT * 3;
			tileClips[TILE_GRASS_CLIFF_RIGHT].w = TILE_WIDTH;
			tileClips[TILE_GRASS_CLIFF_RIGHT].h = TILE_HEIGHT;

			tileClips[TILE_PORTAL].x = TILE_WIDTH;
			tileClips[TILE_PORTAL].y = 0;
			tileClips[TILE_PORTAL].w = TILE_WIDTH;
			tileClips[TILE_PORTAL].h = TILE_HEIGHT;

			tileClips[TILE_GRASS_LEFT_ROUNDED].x = TILE_WIDTH * 4;
			tileClips[TILE_GRASS_LEFT_ROUNDED].y = TILE_HEIGHT * 3;
			tileClips[TILE_GRASS_LEFT_ROUNDED].w = TILE_WIDTH;
			tileClips[TILE_GRASS_LEFT_ROUNDED].h = TILE_HEIGHT;

			tileClips[TILE_GRASS_CENTER].x = TILE_WIDTH * 5;
			tileClips[TILE_GRASS_CENTER].y = TILE_HEIGHT * 3;
			tileClips[TILE_GRASS_CENTER].w = TILE_WIDTH;
			tileClips[TILE_GRASS_CENTER].h = TILE_HEIGHT;

			tileClips[TILE_GRASS_RIGHT_ROUNDED].x = TILE_WIDTH * 6;
			tileClips[TILE_GRASS_RIGHT_ROUNDED].y = TILE_HEIGHT * 3;
			tileClips[TILE_GRASS_RIGHT_ROUNDED].w = TILE_WIDTH;
			tileClips[TILE_GRASS_RIGHT_ROUNDED].h = TILE_HEIGHT;

			tileClips[TILE_MOVING_PLATFORM].x = TILE_WIDTH * 2;
			tileClips[TILE_MOVING_PLATFORM].y = 0;
			tileClips[TILE_MOVING_PLATFORM].w = TILE_WIDTH;
			tileClips[TILE_MOVING_PLATFORM].h = TILE_HEIGHT;

			tileClips[TILE_MOVING_PLATFORM_STOP].x = TILE_WIDTH * 3;
			tileClips[TILE_MOVING_PLATFORM_STOP].y = 0;
			tileClips[TILE_MOVING_PLATFORM_STOP].w = TILE_WIDTH;
			tileClips[TILE_MOVING_PLATFORM_STOP].h = TILE_HEIGHT;

			tileClips[TILE_SPIKES].x = TILE_WIDTH * 4;
			tileClips[TILE_SPIKES].y = 0;
			tileClips[TILE_SPIKES].w = TILE_WIDTH;
			tileClips[TILE_SPIKES].h = TILE_HEIGHT;

			tileClips[TILE_ICE_WHOLE].x = TILE_WIDTH * 5;
			tileClips[TILE_ICE_WHOLE].y = 0;
			tileClips[TILE_ICE_WHOLE].w = TILE_WIDTH;
			tileClips[TILE_ICE_WHOLE].h = TILE_HEIGHT;

			tileClips[TILE_ICE_BROKEN_1].x = TILE_WIDTH * 6;
			tileClips[TILE_ICE_BROKEN_1].y = 0;
			tileClips[TILE_ICE_BROKEN_1].w = TILE_WIDTH;
			tileClips[TILE_ICE_BROKEN_1].h = TILE_HEIGHT;

			tileClips[TILE_ICE_BROKEN_2].x = TILE_WIDTH * 7;
			tileClips[TILE_ICE_BROKEN_2].y = 0;
			tileClips[TILE_ICE_BROKEN_2].w = TILE_WIDTH;
			tileClips[TILE_ICE_BROKEN_2].h = TILE_HEIGHT;

			tileClips[TILE_ICE_BROKEN_3].x = TILE_WIDTH * 8;
			tileClips[TILE_ICE_BROKEN_3].y = 0;
			tileClips[TILE_ICE_BROKEN_3].w = TILE_WIDTH;
			tileClips[TILE_ICE_BROKEN_3].h = TILE_HEIGHT;

			tileClips[TILE_SAW_1].x = TILE_WIDTH;
			tileClips[TILE_SAW_1].y = TILE_HEIGHT;
			tileClips[TILE_SAW_1].w = 120;
			tileClips[TILE_SAW_1].h = 120;
		}
	}
	//Close the file
	map.close();

	return tilesLoaded;

}

void LevelCreator::putTile(int mouseX, int mouseY) {
	for (int i = 0; i < TOTAL_TILES; i++) {
		SDL_Rect box = tileSet[i]->getBox();
		if ((mouseX > box.x) && (mouseX < box.x + box.w) && (mouseY > box.y) && (mouseY < box.y + box.h)) {
			delete tileSet[i];
			tileSet[i] = new Tile(box.x, box.y, currentType);
		}
	}
}


void LevelCreator::saveTiles(std::string levelPath) {
	//Open the map file
	std::ofstream map(levelPath.c_str());

	for (int i = 0; i < TOTAL_TILES; i++) {
		map << tileSet[i]->getType() << " ";
	}
	//Close the map file
	map.close();
}

void LevelCreator::renderLevel(Sprite tileTexture, SDL_Renderer* renderer) {
	//renders the level
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		//if (tileSet[i]->getType() == TILE_MOVING_PLATFORM) {
		//	tileSet[i]->movePlatform(tileSet);
		//}
		tileSet[i]->render(tileTexture, renderer, tileClips); //renders the tiles to the renderer from the tile set vector
	}
}