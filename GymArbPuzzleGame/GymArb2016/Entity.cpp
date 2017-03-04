#include "Entity.h"



Entity::Entity()
{
	//Initialize the variables
	

	m_texture = nullptr;

	m_posRect.x = m_posRect.y = m_posRect.w = m_posRect.h = 0;

	
}


Entity::~Entity()
{
	//Deallocate
	free();
}

void Entity::render(int xPos, int yPos, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	//Sets the x and y position
	m_posRect.x = xPos;
	m_posRect.y = yPos;

	//Renderes the entity
	SDL_RenderCopyEx(m_renderer, m_texture, clip, &m_posRect, angle, center, flip);
}

void Entity::setSprite(std::string path) {
	//Creates a temporary surface, easier to manipulate
	SDL_Surface* tempSurface = IMG_Load(path.c_str());
	
	//If the surface is not valid
	if (tempSurface == nullptr) {
		std::cout << "Could not load surface! \n";
	}
	else {
		SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 0xdb, 0xdb, 0xdb));
		m_texture = SDL_CreateTextureFromSurface(m_renderer, tempSurface);
		SDL_QueryTexture(m_texture, NULL, NULL, &m_posRect.w, &m_posRect.h);
	}

	//Deallocate the surface
	SDL_FreeSurface(tempSurface);
}


void Entity::free() {
	//Destoy and reset the texture
	if (m_texture != nullptr) {
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
		m_posRect.x = m_posRect.y = m_posRect.w = m_posRect.h = 0;

	}
	//Deallocate the entity
	delete this;
}

bool Entity::checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

bool Entity::checkCircularCollision(SDL_Rect player, SDL_Rect circle, float velX, float velY)
{
	//Closest point on collision box
	int cX, cY;

	//Find closest x offset
	if (player.x < circle.x)
	{
		cX = circle.x;
	}
	else if (player.x > circle.x + circle.w)
	{
		cX = circle.x + circle.w;
	}
	else
	{
		cX = player.x;
	}
	if (player.y < circle.y)
	{
		cY = circle.y;
	}
	else if (player.y > circle.y + circle.h)
	{
		cY = circle.y + circle.h;
	}
	else
	{
		cY = player.y;
	}

	//if the closest point is inside the circle
	if (((cX - circle.x) * (cX - circle.x) + (cY - circle.y) * (cY - circle.y)) < circle.w / 2 * circle.w / 2)
	{
		//This rect and the circle have collided
		return true;
	}



	//If the shapes have not collided
	return false;
}

bool Entity::circularCollision(SDL_Rect circle, SDL_Rect rect)
{

	//Closest point on collision box
	int cX, cY;

	//Find closest x offset
	if (circle.x + circle.w < rect.x)
	{
		cX = rect.x;
	}
	else if (circle.x + circle.w > rect.x + rect.w)
	{
		cX = rect.x + rect.w;
	}
	else
	{
		cX = circle.x + circle.w;
	}

	//Find closest y offset
	if (circle.y + circle.h < rect.y)
	{
		cY = rect.y;
	}
	else if (circle.y + circle.h > rect.y + rect.h)
	{
		cY = rect.y + rect.h;
	}
	else
	{

		cY = circle.y + circle.h;
	}
	//If the closest point is inside the circle
	/*if (distanceSquared(circle.x + circle.w, circle.y + circle.h, cX, cY) < circle.w / 2 * circle.w / 2)
	{
		//This box and the circle have collided
		return true;
	}*/

	return false;
}


////Setterse and getters ////
SDL_Rect Entity::getBox() {
	return m_posRect;
}
SDL_Texture* Entity::getSprite() {
	return m_texture;
}
int Entity::getWidth(){
	return m_posRect.w; 
}
int Entity::getHeight() {
	return m_posRect.h;
}
int Entity::getX() {
	return m_posRect.x;
}
int Entity::getY() {
	return m_posRect.y;
}
float Entity::getAngle() {
	return m_angle; 
}