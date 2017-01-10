/*#include "Circle.h"

Circle::Circle(SDL_Renderer *renderer, float radi, float angle)
{
	m_step = 1;
	m_theta = 0;
	m_radi = radi;
	m_gravity = 0.2;

	m_speed = 4;

	m_angle = angle;

	m_velX = (m_speed *(cos(angle*M_PI / 180)));

	m_velX = 0;
	m_velY = (m_speed * (sin(angle*M_PI / 180)));
	m_velY = 0;

	m_imgRect.x = 100; 
	m_imgRect.y = 100;

}

Circle::~Circle()
{
}


void Circle::render(Sprite &circleTexture, SDL_Renderer* renderer) {
	/*for (int i = 0; i < 360; i += m_step)
	{
		float x = m_xPos + m_radi * cos(i);
		float y = m_yPos + m_radi * sin(i);
		SDL_SetRenderDrawColor(m_gameRenderer, 255, 0, 0, 255);
		SDL_RenderDrawPoint(m_gameRenderer, x, y);
	}*/
	
	/*//puts the circle on the renderer
	SDL_QueryTexture(circleTexture.m_texture, NULL, NULL, &m_imgRect.w, &m_imgRect.h);
	circleTexture.render(renderer, m_imgRect.x, m_imgRect.y);
}

void Circle::move(Tile * tiles[], Player player)
{
	xPos = m_imgRect.x;
	yPos = m_imgRect.y;
	//if he circle hits the right side of the window
	if (m_imgRect.x + m_imgRect.w  >= SCREEN_WIDTH) {
		m_imgRect.x = SCREEN_WIDTH - m_imgRect.w;
		m_velX = -m_velX;
	}

	//if the circle hits the bottom of the window
	if (m_imgRect.y + m_imgRect.h >= SCREEN_HEIGHT) {
		m_imgRect.y = SCREEN_HEIGHT - m_imgRect.h;
		if (m_velY >= 1.3)
			m_velY = -m_velY * 0.5;
		else
			m_velY = 0;

	}
	//if circle hits the left side
	if (m_imgRect.x <= 0) {
		m_velX = -m_velX;
	}

	//if circle hits the roof
	if (m_imgRect.y <= 0) {
		m_velY = -m_velY;
	}

	m_imgRect.x += m_velX;
	m_imgRect.y += m_velY;

	m_angle += m_velX;

	if (m_velX < -0.5 || m_velX > 0.5)
		m_velX *= 0.99;
	else
		m_velX = 0;

	m_velY += m_gravity;

	
	
	for (int i = 0; i < TOTAL_TILES; i++)
	{
		
		if (tiles[i]->getType() >= TILE_BRIDGE) {

			//collision with solid blocks

			if (checkCollision(m_imgRect, tiles[i]->getBox()))
			{
				
				//if the circle is above the rectangle
				if (yPos + m_imgRect.h <= tiles[i]->getBox().y)
				{
					

						//checks the y speed of the circle, this helps deciding if the ball should bounce or not
						if (m_velY >= 3)
							m_velY = -m_velY * 0.5; //the ball bounces
						else {
							m_velY = 0; // the ball does not bounce
							
						}
						m_imgRect.y = tiles[i]->getBox().y - m_imgRect.h;
					
				}

				//if the rect is on the right of the block
				else if (xPos >= tiles[i]->getBox().x + tiles[i]->getBox().w) {
					if (tiles[i]->getType() != TILE_BRIDGE) {
						m_imgRect.x = tiles[i]->getBox().x + tiles[i]->getBox().w;
						m_velX = -m_velX;
					}
				}

				//if the circle is on the left of the block
				else if (xPos + m_imgRect.w <= tiles[i]->getBox().x) {
					if (tiles[i]->getType() != TILE_BRIDGE) {
						m_imgRect.x = tiles[i]->getBox().x - m_imgRect.w;
						m_velX = -m_velX;
					}
				}

				//if the circle is under the rectangle
				else if (yPos >= tiles[i]->getBox().y + tiles[i]->getBox().h) {
					if (tiles[i]->getType() != TILE_BRIDGE) {
						m_imgRect.y = tiles[i]->getBox().y + tiles[i]->getBox().h;
						m_velY = -m_velY;
					}
				}
			}
		}
	}
	
	
	if (playerCollision(player)) {


		//if the circle is above the rectangle
		if (yPos + m_imgRect.h <= player.getBox().y)
		{


			//checks the y speed of the circle, this helps deciding if the ball should bounce or not
			if (m_velY >= 3)
				m_velY = -m_velY * 0.5; //the ball bounces
			else {
				m_velY = 0; // the ball does not bounce

			}
			m_imgRect.y = player.getBox().y - m_imgRect.h;

		}

		//if the rect is on the right of the block
		else if (xPos >= player.getBox().x + player.getBox().w) {

			m_imgRect.x = player.getBox().x + player.getBox().w;
			m_velX = -m_velX;

		}

		//if the circle is on the left of the block
		else if (xPos + m_imgRect.w <= player.getBox().x) {

			m_imgRect.x = player.getBox().x - m_imgRect.w;
			m_velX = -m_velX;

		}

		//if the circle is under the rectangle
		else if (yPos >= player.getBox().y + player.getBox().h) {

			m_imgRect.y = player.getBox().y + player.getBox().h;
			m_velY = -m_velY;

		}
	}
}

/*
bool Circle::checkCollision(Tile* tile)
{
	float cX, cY;
	
	//find the closest x offset
	if (m_imgRect.x < tile->getBox().x)
		cX = tile->getBox().x;
	else if (m_imgRect.x > tile->getBox().x + tile->getBox().w)
		cX = tile->getBox().x + tile->getBox().w;
	else
		cX = m_imgRect.x;

	//fins the closest y offset
	if (m_imgRect.y + m_velY < tile->getBox().y)
		cY = tile->getBox().y;

	else if (m_imgRect.y + m_velY> tile->getBox().y + tile->getBox().h)
		cY = tile->getBox().y + tile->getBox().h;
	else
		cY = m_imgRect.y;

	//if the closest point is inside the circle
	if (((cX - m_imgRect.x) * (cX - m_imgRect.x) + (cY - m_imgRect.y) * (cY - m_imgRect.y)) < m_radi * m_radi)
	{
		//This rect and the circle have collided
		return true;
	}

	//if the shapes have not collided
	return false;

}
*//*
bool Circle::playerCollision(Player player) {
	float cX, cY;

	//find the closest x offset
	if (xPos < player.getBox().x)
		cX = player.getBox().y;
	else if (xPos > player.getBox().y + player.getBox().w)
		cX = player.getBox().y + player.getBox().w;
	else
		cX = xPos;

	//fins the closest y offset
	if (yPos + m_velY < player.getBox().y)
		cY = player.getBox().y;

	else if (yPos + m_velY > player.getBox().y + player.getBox().h)
		cY = player.getBox().y + player.getBox().h;
	else
		cY = yPos;

	//if the closest point is inside the circle
	if (((cX - xPos) * (cX - xPos) + (cY - yPos) * (cY - yPos)) < m_radi * m_radi)
	{
		//This rect and the circle have collided
		return true;
	}

	//if the shapes have not collided
	return false;
}

*/