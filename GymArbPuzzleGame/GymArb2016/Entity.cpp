#include "Entity.h"



Entity::Entity(SDL_Renderer* renderer)
{
	//Initialize the variables
	m_renderer = renderer;

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