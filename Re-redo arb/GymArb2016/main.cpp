#include <iostream>
#include <SDL/SDL.h>
#include <sdl/SDL_image.h>
#include <string.h>
#include <iostream>
#include <fstream>

//the dimensions of the screen
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//the dimensions of the level
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

//tile constants
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 3;

//the different tile sprites
const int TILE_RED = 1;
const int TILE_GREEN = 2;
const int TILE_BLUE = 0;

class Texture
{
public:

	//Initializes variables
	Texture();

	//Deallocates memory
	~Texture();

	//loads image at specified path
	bool loadFromFile(std::string path);

	//deallocates texture
	void free();

	//renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//the actual hardware texture
	SDL_Texture* m_texture;

	//image dimensions
	int m_width;
	int m_height;
};


class Tile
{
public:
	//initializes position and type
	Tile(int x, int y, int tileType);

	//shows the tile
	void render(SDL_Rect& camera);

	//get the tile type
	int getType();

	//get the collision box
	SDL_Rect getBox();

private:
	//the attributes of the tile
	SDL_Rect m_box;

	//the tile type
	int m_type;
};

//starts up SDL and creates window
bool init();

//loads media, will initialize tiles so it needs tiles as parameter
bool loadMedia(Tile* tiles[]);

//frees media and shuts down SDL
void close(Tile* tiles[]);

//box collision detector
bool checkCollision(SDL_Rect a, SDL_Rect b);

//sets tiles from tile map
bool setTiles(Tile* tiles[]);

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

//scene textures
Texture tileTexture;
SDL_Rect tileClips[TOTAL_TILE_SPRITES];


Texture::Texture()
{
	m_texture = nullptr;
	m_width = 0;
	m_height = 0;
}

Texture::~Texture()
{
	//deallocate
	free();
}

bool Texture::loadFromFile(std::string path)
{
	//get rid of preexisting texture
	free();

	m_texture = IMG_LoadTexture(renderer, path.c_str());

	SDL_QueryTexture(m_texture, NULL, NULL, &m_width, &m_height);

	
	return m_texture != nullptr;
}

void Texture::free()
{
	if (m_texture != nullptr) {
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
		m_width = 0;
		m_height = 0;
	}
}

void Texture::render(int x, int y, SDL_Rect* clip /* = NULL */, double angle /* = 0.0 */, SDL_Point* center /* = NULL */, SDL_RendererFlip flip /* = SDL_FLIP_NONE */)
{
	//set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, m_width, m_height };

	//set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//render to screen
	SDL_RenderCopyEx(renderer, m_texture, clip, &renderQuad, angle, center, flip);
	
}

int Texture::getWidth()
{
	return m_width;
}

int Texture::getHeight()
{
	return m_height;
}

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
}

void Tile::render(SDL_Rect& camera)
{
	//the the tile is on screen
	if(checkCollision(camera, m_box)){

		//show the tile
		tileTexture.render(m_box.x - camera.x, m_box.y - camera.y, &tileClips[m_type]);
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

bool init()
{
	//initialization flag
	bool success = true;
	//Initializing SDL
	SDL_Init(SDL_INIT_VIDEO);
	if (SDL_Init == nullptr)
		std::cout << "Error: " << SDL_GetError() << std::endl;

	//Initializing SDL_Image
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(IMG_INIT_PNG) && imgFlags))
		std::cout << "Error: " << IMG_GetError();

	

	//Creates the game window
	window = SDL_CreateWindow("Puzzle game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
		std::cout << "Error: " << SDL_GetError() << std::endl;

	//Creates the game renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
		std::cout << "Error: " << SDL_GetError() << std::endl;

	return success;
}

bool loadMedia(Tile* tiles[])
{
	//loading success flag
	bool success = true;

	//load tile texture
	if (!tileTexture.loadFromFile("tiles/tiles.png")) {
		std::cout << "Failed to load tile set texture!" << std::endl;
		success = false;
	}

	//Load tile map
	if (!setTiles(tiles)) {
		std::cout << "Failed to load tile set ! " << std::endl;
		success = false;
	}

	return success;
}

void close(Tile* tiles[])
{
	//deallocate tiles
	for (int i = 0; i < TOTAL_TILES; i++)
	{
		if (tiles[i] == NULL) {
			delete tiles[i];
			tiles[i] = NULL;
		}
	}

	//free loaded images
	tileTexture.free();

	//destroy window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = nullptr;
	renderer = nullptr;

	//quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool checkCollision(SDL_Rect a, SDL_Rect b)
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


bool setTiles(Tile* tiles[])
{
	//success flag
	bool tilesLoaded = true;

	//get tile offsets
	int x = 0;
	int y = 0;

	//open the map
	std::ifstream map("lazy.map");

	//if the map couldn't be loaded 
	if (!map.is_open()) {
		std::cout << "Unable to load map file!" << std::endl;
		tilesLoaded = false;
	}
	else {
		//initialize the tiles
		for (int i = 0; i < TOTAL_TILES; i++)
		{
			//determines what kind of tile will be made
			int tileType = -1;
	
			//red tile from map file
			map >> tileType;
			
			//if there was a problem reading the map
			if (map.fail()) {
				std::cout << "Error loading map! " << std::endl;
				tilesLoaded = false;
				break;
			}
		
			if ((tileType >= 0) && tileType < TOTAL_TILE_SPRITES) {
				tiles[i] = new Tile(x, y, tileType);
			}
			//if the don't recognize the tile type
			else {
				std::cout << "Error loading map! Invalid tile type" << std::endl;
				tilesLoaded = false;
				break;
			}

			//move to the next tile spot
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
			tileClips[TILE_RED].x = 0;
			tileClips[TILE_RED].y = 0;
			tileClips[TILE_RED].w = TILE_WIDTH;
			tileClips[TILE_RED].h = TILE_HEIGHT;

			tileClips[TILE_GREEN].x = 0;
			tileClips[TILE_GREEN].y = 80;
			tileClips[TILE_GREEN].w = TILE_WIDTH;
			tileClips[TILE_GREEN].h = TILE_HEIGHT;

			tileClips[TILE_BLUE].x = 0;
			tileClips[TILE_BLUE].y = 160;
			tileClips[TILE_BLUE].w = TILE_WIDTH;
			tileClips[TILE_BLUE].h = TILE_HEIGHT;

			
		}
	}
	//Close the file
	map.close();


	return tilesLoaded;

}

int main(int argc, char *argv[]) {
	if (!init()) {
		std::cout << "Failed to initialize!" << std::endl;
	}
	else {
		//the level tiles
		Tile* tileSet[TOTAL_TILES];

		//load media
		if (!loadMedia(tileSet)) {
			std::cout << "failed to load media!" << std::endl;
		}

		else {
			bool quit = false;

			SDL_Event evnt;

			SDL_Rect camera = { 0,0, SCREEN_WIDTH, SCREEN_HEIGHT };

			while (!quit) {
				while (SDL_PollEvent(&evnt)) {
					if (evnt.type == SDL_QUIT) {
						quit = true;
					}
				}

				//clear screen
				SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
				SDL_RenderClear(renderer);

				//render level
				for (int i = 0; i < TOTAL_TILES; i++)
				{
					tileSet[i]->render(camera);
				}

				SDL_RenderPresent(renderer);
			}
		}
		close(tileSet);
	}
	
	int test;
	std::cin >> test;
	return 0;
}


