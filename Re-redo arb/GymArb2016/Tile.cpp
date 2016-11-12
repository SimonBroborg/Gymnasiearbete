#include "Tile.h"
#include <SDL\SDL_image.h>
#include "Manager.h"
#include <iostream>
#include "Player.h"
#include <fstream>
#include "Sprite.h"

Tile::Tile()
{
}


Tile::~Tile()
{
}


SDL_Texture * Tile::loadTile(SDL_Renderer *renderer, std::string path)
{
	SDL_Texture * m_texture = IMG_LoadTexture(renderer, path.c_str()); //loads sprite from file path
	return  m_texture;
}


void Tile::createMap(SDL_Renderer *renderer, int map[10][18], std::vector<SDL_Rect> &rRects)
{
	Manager game;
	for (int i = 0; i < X_TILES; i++)
	{
		for (int j = 0; j < Y_TILES; j++)
		{
			//checks if the chosen position got a tile (1) and if so it creates a rect at that position of the map
			switch (map[j][i])
			{
				//block which the player collides with from all sides
			case 1:
				tileSprite = loadTile(renderer, "tiles/box.png");
				SDL_QueryTexture(tileSprite, NULL, NULL, &tileRect.w, &tileRect.h);

				tileRect.x = TILE_WIDTH * i;
				tileRect.y = TILE_HEIGHT * j;


				break;

				//platform in the upper part of the tile which the player only collides with from the top
			case 2:
				tileSprite = loadTile(renderer, "tiles/bridge.png");
				SDL_QueryTexture(tileSprite, NULL, NULL, &tileRect.w, &tileRect.h);

				tileRect.x = TILE_WIDTH * i;
				tileRect.y = TILE_HEIGHT * j;

			
				break;

				//platform in the lower part of the tile which the player only collides with from the top
			case 3:
				tileSprite = loadTile(renderer, "tiles/bridge.png");
				SDL_QueryTexture(tileSprite, NULL, NULL, &tileRect.w, &tileRect.h);

				tileRect.x = TILE_WIDTH * i;
				tileRect.y = TILE_HEIGHT * j;

				system("CLS");
				break;

			}

			rRects.push_back(tileRect); //takes the created rect and puts it into the rects vector

			tileRect = { 0,0,0,0 };
		}
	}

}



void Tile::render(SDL_Rect tileRect) 
{
	tileSprite = loadTile(gameRenderer, "tiles/bridge.png");
	SDL_RenderCopy(gameRenderer, tileSprite, NULL, &tileRect);
	system("CLS");
}