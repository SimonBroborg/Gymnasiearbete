#include "Tile.h"
#include <SDL\SDL_image.h>
#include <iostream>
#include "Sprite.h"

#include <fstream>



Tile::Tile(int x, int y, int w, int h, int tileType)
{
	//get the offsets
	m_box.x = x;
	m_box.y = y;

	
	//set the collision box
	m_box.w = w;
	m_box.h = h;

	

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


void Tile::render(Sprite &tileTexture, SDL_Renderer* gameRenderer, SDL_Rect tileClips[TOTAL_TILE_SPRITES])
{
	//show the tile
		if (m_type == TILE_SAW_1) {
			rotate();
		}

		tileTexture.render(gameRenderer, m_box.x, m_box.y, &tileClips[m_type], angle);

		if (tileTexture.m_texture == nullptr)
			std::cout << "tileTexture.m_texture == nullptr";
	

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
			/*if (tiles[i]->getType() == TILE_MOVING_PLATFORM_STOP)
				m_velX = -m_velX;*/
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

void Tile::setType(int type) {
	this->m_type = type;
}

