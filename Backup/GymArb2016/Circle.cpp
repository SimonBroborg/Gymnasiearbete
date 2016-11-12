#include "Circle.h"
#include "Player.h"

Circle::Circle(SDL_Renderer *renderer, float radi, float angle)
{
	m_step = 1;
	m_theta = 0;
	m_radi = radi;
	m_gameRenderer = renderer;
	m_gravity = 0.2;

	m_speed = 4;

	m_angle = angle;

	m_velX = (m_speed *(cos(angle*M_PI / 180)));

	m_velX = 0;
	m_velY = (m_speed * (sin(angle*M_PI / 180)));
	m_velY = 0;

	std::cout << m_scaleX << std::endl;
	std::cout << angle << std::endl;

	m_imgRect.w = m_imgRect.h = (m_radi) * 2;

	screenWidth = 1280;
	screenHeight = 700;

}

Circle::~Circle()
{
}


void Circle::draw() {
	/*for (int i = 0; i < 360; i += m_step)
	{
		float x = m_xPos + m_radi * cos(i);
		float y = m_yPos + m_radi * sin(i);
		SDL_SetRenderDrawColor(m_gameRenderer, 255, 0, 0, 255);
		SDL_RenderDrawPoint(m_gameRenderer, x, y);
	}*/
	
	//sets the circle picture on the circle
	m_imgRect.x = xPos - m_radi;
	m_imgRect.y = yPos - m_radi;

	//puts the circle on the renderer
	SDL_RenderCopyEx(m_gameRenderer, m_texture, NULL, &m_imgRect, m_angle, NULL, SDL_FLIP_NONE);
}

void Circle::move(std::vector<SDL_Rect> &rects, SDL_Rect &player, float playerVelX)
{
	Manager game;


	//if he circle hits the right side of the window
	if (xPos + m_radi + m_velX > screenWidth) {
		m_velX = -m_velX;
	}

	//if the circle hits the bottom of the window
	if (yPos + m_radi + m_velY > screenHeight) {
		if (m_velY >= 1.3)
			m_velY = -m_velY * 0.5;
		else
			m_velY = 0;

	}
	//if circle hits the left side
	if (xPos - m_radi + m_velX < 0) {
		m_velX = -m_velX;

	}
	//if circle hits the roof
	if (yPos - m_radi + m_velY < 0) {
		m_velY = -m_velY;
	}

	xPos += m_velX;
	yPos += m_velY;

	m_angle += m_velX;

	if (m_velX < -0.5 || m_velX > 0.5)
		m_velX *= 0.99;
	else
		m_velX = 0;

	m_velY += m_gravity;
	

	for (int i = 0; i < rects.size(); i++)
	{
		SDL_Rect &rect = rects[i];

		//collision with solid blocks
		
			if (checkCollision(rect))
			{

				//if the circle is above the rectangle
				if (yPos < rect.y)
				{
					//checks if the x position of the circle is inside the rectangle
					if (xPos <= rect.x + rect.w && xPos > rect.x) {
						yPos = rect.y - m_radi;

						//checks the y speed of the circle, this helps deciding if the ball should bounce or not
						if (m_velY >= 1.3)
							m_velY = -m_velY * 0.5; //the ball bounces
						else
							m_velY = 0; // the ball does not bounce
					}

					//what happens if the ball collides from an angle thats not 90, 180, 270 or 360 degrees
					else 
					{
						m_velY = 0; //stops the ball y motion
					}
				}

				//if the rect is on the right of the block
				else if (xPos > rect.x + rect.w) {
					if (rect.h == 32) {
						xPos = rect.x + rect.w + m_radi;
						m_velX = -m_velX;
					}
				}

				//if the circle is on the left of the block
				else if (xPos < rect.x) {
					if (rect.h == 32) {
						xPos = rect.x - m_radi;
						m_velX = -m_velX;
					}
				}

				//if the circle is under the rectangle
				else if (yPos > rect.y + rect.h)
					if(rect.h == 32)
					m_velY = -m_velY;
			}
	}
	

	if (playerCollision(player, playerVelX)) {


		//if the circle is above the rectangle
		if (yPos < player.y)
		{
			//checks if the x position of the circle is inside the rectangle
			if (xPos <= player.x + player.w && xPos > player.x) {
				yPos = player.y - m_radi;

				//checks the y speed of the circle, this helps deciding if the ball should bounce or not
				if (m_velY >= 1.3)
					m_velY = -m_velY * 0.5; //the ball bounces
				else
					m_velY = 0; // the ball does not bounce
			}

			//what happens if the ball collides from an angle thats not 90, 180, 270 or 360 degrees
			else
			{
				m_velY = 0; //stops the ball y motion
			}
		}
		
		//if the rect is on the right of the block
		else if (xPos > player.x + player.w) {
				xPos = player.x + player.w + m_radi;
				m_velX = playerVelX;
			
		}

		//if the circle is on the left of the block
		else if (xPos < player.x) {
				xPos = player.x - m_radi;
				m_velX = playerVelX;
		}

		//if the circle is under the rectangle
		else if (yPos > player.y + player.h)
				m_velY = -m_velY;
	}
	

}

bool Circle::checkCollision(SDL_Rect &rect)
{
	float cX, cY;
	
	//find the closest x offset
	if (xPos < rect.x)
		cX = rect.x;
	else if (xPos > rect.x + rect.w)
		cX = rect.x + rect.w;
	else
		cX = xPos;

	//fins the closest y offset
	if (yPos + m_velY < rect.y)
		cY = rect.y;

	else if (yPos + m_velY> rect.y + rect.h)
		cY = rect.y + rect.h;
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

bool Circle::playerCollision(SDL_Rect &player, float playerVelX) {
	float cX, cY;

	//find the closest x offset
	if (xPos < player.x)
		cX = player.x;
	else if (xPos > player.x + player.w)
		cX = player.x + player.w;
	else
		cX = xPos;

	//fins the closest y offset
	if (yPos + m_velY < player.y)
		cY = player.y;

	else if (yPos + m_velY > player.y + player.h)
		cY = player.y + player.h;
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