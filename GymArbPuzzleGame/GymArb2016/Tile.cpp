#include "Tile.h"
#include <SDL\SDL_image.h>
#include <iostream>
#include "Sprite.h"
#include "Player.h"
#include <fstream>



Tile::Tile(int x, int y, int tileType)
{
	//get the offsets
	m_box.x = x;
	m_box.y = y;

	//set the collision box
	m_box.w = TILE_WIDTH;
	m_box.h = TILE_HEIGHT;


	//get the tile type
	m_type = tileType;

	m_velX = 2; 
	m_velY = 2; 
	m_damage = 0; 

	angle = 0;
}


Tile::~Tile()
{
}


void Tile::render(Sprite &tileTexture, SDL_Renderer* gameRenderer, SDL_Rect tileClips[TOTAL_TILE_SPRITES], SDL_Rect& camera)
{	
	//show the tile
	if (checkCollision(camera, m_box)) {
		if (m_type == TILE_SAW_1) {
			rotate();
		}

		tileTexture.render(gameRenderer, m_box.x - camera.x , m_box.y - camera.y, &tileClips[m_type], angle);

		if (tileTexture.m_texture == nullptr)
			std::cout << "tileTexture.m_texture == nullptr";
	}

}

int Tile::getType()
{
	return m_type;
}

SDL_Rect Tile::getBox()
{
	return m_box;
}


void Tile::movePlatform(Tile* tiles[])
{
	m_box.x += m_velX;

	for (int i = 0; i < TOTAL_TILES; i++) {
		if (checkCollision(m_box, tiles[i]->getBox())) {
			if(tiles[i]->getType() == TILE_MOVING_PLATFORM_STOP)
				m_velX = -m_velX;
		}
	}
	
		
}

void Tile::rotate()
{
	angle += 10;
	if (angle == 360)
		angle = 0; 
}


void Tile::destroy(float delta)
{
	
	m_damage += delta * 1000; 

	if (m_damage > 50) {
		if (m_type == TILE_ICE_WHOLE)
			m_type++;
		else if (m_type == TILE_ICE_BROKEN_1)
			m_type++;
		else if (m_type == TILE_ICE_BROKEN_2)
			m_type++;
		else if (m_type == TILE_ICE_BROKEN_3) {
			delete this;
		}
	}

	
}

float Tile::getSpeed() {
	return m_velX;
}

/*
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

}*/


