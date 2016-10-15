#include "Circle.h"

Circle::Circle(SDL_Renderer *renderer, float xPos, float yPos, float radi, float angle)
{
	m_step = 1;
	m_theta = 0;
	m_xPos = xPos;
	m_yPos = yPos;
	m_radi = radi;
	m_gameRenderer = renderer;
	m_gravity = 0.5;

	m_speed = 4;

	m_angle = angle;

	

	m_velX = (m_speed *(cos(angle*M_PI/180)));

	m_velX = -4;
	m_velY = (m_speed * (sin(angle*M_PI/180)));
	m_velY = 0;

	std::cout << m_scaleX << std::endl;
	std::cout << angle << std::endl;


	
	SDL_Surface *surface = IMG_Load("circle2.png");
	if (surface == nullptr)
	{
		std::cout << "Could not load image!" << std::endl;
	}

	texture = SDL_CreateTextureFromSurface(m_gameRenderer, surface);

	SDL_FreeSurface(surface);
	
	m_imgRect.w = m_imgRect.h = (m_radi +1) * 2 ;
	
}

Circle::~Circle()
{
}


void Circle::drawCircle() {
	/*for (int i = 0; i < 360; i += m_step)
	{
		int x = m_xPos + m_radi * cos(i);
		int y = m_yPos + m_radi * sin(i);
		SDL_SetRenderDrawColor(m_gameRenderer, 255, 0, 0, 255);
		SDL_RenderDrawPoint(m_gameRenderer, x, y);
	}*/

	m_imgRect.x = m_xPos - m_radi;
	m_imgRect.y = m_yPos - m_radi;

	SDL_RenderCopyEx(m_gameRenderer, texture, NULL, &m_imgRect, m_angle, NULL, SDL_FLIP_NONE);
}

void Circle::move()
{
	Manager game;

	
	if (m_xPos + m_radi + m_velX > game.SCREEN_WIDTH) {
		m_velX = -m_velX;
	}
	if (m_yPos + m_radi + m_velY > game.SCREEN_HEIGHT) {
		if (m_velY >= 1.3) 
			m_velY = -m_velY * 0.5;
		else
			m_velY = 0;
			
	}
	if (m_xPos - m_radi + m_velX < 0) {
		m_velX = -m_velX;

	}
	if (m_yPos - m_radi + m_velY < 0) {
		m_velY = -m_velY;
	}
	m_xPos += m_velX;
	m_yPos += m_velY;
	
	m_angle += m_velX;

	if (m_velX < -0.5 || m_velX > 0.5)
		m_velX *= 0.99;
	else
		m_velX = 0;
	m_velY += m_gravity;


	

	
}

void Circle::circleCollision(Circle A, Circle B)
{
	static int i = 0;
	float distance = sqrt(pow(A.m_xPos - B.m_xPos, 2) + pow(A.m_yPos - B.m_yPos, 2));
	float radiDistance = A.m_radi + B.m_radi;
	
	if (distance < radiDistance) {

		A.m_velX = 0;
		
		std::cout << "Collision! " << i << std::endl;
		i++;
		
	} 
}