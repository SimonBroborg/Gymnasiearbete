#include "Tile.h"
#include <SDL\SDL_image.h>
#include "Manager.h"
#include <iostream>
#include "Player.h"
Tile::Tile()
{
}


Tile::~Tile()
{
}


SDL_Texture * Tile::loadTile(SDL_Renderer *renderer, std::string path)
{
	SDL_Texture *tile= IMG_LoadTexture(renderer, path.c_str()); //loads sprite from file path
	return  tile;
}


void Tile::createMap(SDL_Renderer *renderer, int map[15][20], std::vector<SDL_Rect> &rRects)
{
	Manager game;

	//loops through each possible tile position 
	for (int i = 0; i < X_TILES; i++)
	{
		for (int j = 0; j < Y_TILES; j++)
		{
			//checks if the chosen position got a tile (1) and if so it creates a rect at that position of the map
			switch (map[j][i])
			{
				//block which the player collides with from all sides
			case 1:
				tileSprite = loadTile(renderer, "block.png");
				tileRect.w = 32;
				tileRect.h = 32;
				tileRect.x = 32 * i;
				tileRect.y = 32 * j;

				rRects.push_back(tileRect); //takes the created rect and puts it into the rects vector

				SDL_RenderCopy(renderer, tileSprite, NULL, &tileRect);

				SDL_DestroyTexture(tileSprite);

				break;

				//platform in the upper part of the tile which the player only collides with from the top
			case 2: 
				tileSprite = loadTile(renderer, "platform.png");
				tileRect.w = 32;
				tileRect.h = 8;
				tileRect.x = 32 * i;
				tileRect.y = 32 * j;

				rRects.push_back(tileRect); //takes the created rect and puts it into the rects vector

				SDL_RenderCopy(renderer, tileSprite, NULL, &tileRect);

				SDL_DestroyTexture(tileSprite);

				break;

				//platform in the lower part of the tile which the player only collides with from the top
			case 3:
				tileSprite = loadTile(renderer, "platform.png");
				tileRect.w = 32;
				tileRect.h = 8;
				tileRect.x = 32 * i;
				tileRect.y = 32 * j + 16;

				rRects.push_back(tileRect); //takes the created rect and puts it into the rects vector

				SDL_RenderCopy(renderer, tileSprite, NULL, &tileRect);

				SDL_DestroyTexture(tileSprite);

				break;

			}
		}
	}
}