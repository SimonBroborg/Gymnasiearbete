#include "Manager.h"

Manager::Manager()
{
	bIsRunning = true;

}


Manager::~Manager()
{
}


bool Manager::initSystems() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "SDL_Init error: " << SDL_GetError();
		return 1;
	}
	else {
		std::cout << "SDL_Init successful!" << std::endl;
		globalWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
		if (globalWindow == nullptr) {
			std::cout << "SDL_CreateWindow error: " << SDL_GetError();
			return 1;
		}
		else {
			std::cout << "SDL_CreateWindow successful!" << std::endl;
			globalRenderer = SDL_CreateRenderer(globalWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (globalRenderer == nullptr) {
				std::cout << "SDL_CreateRenderer error: " << SDL_GetError();
				return 1;
			}
			else {
				std::cout << "SDL_CreateRenderer successful!" << std::endl;
				if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
					std::cout << "SDL_Init error: " << SDL_GetError();
					return 1;
				}
				else
				{
					std::cout << "IMG_Init successful!" << std::endl;
					if (TTF_Init() < 0)
					{
						std::cout << "SDL_ttf could not be initialized!" << std::endl;
						return 1;
					}
					std::cout << "TTF_init successfull!" << std::endl;
				}
			}
		}

	}

	return 1;

}

bool Manager::loadGame() {

	return true;
}


SDL_Texture* Manager::loadTexture(std::string path) {
	SDL_Texture* texture = IMG_LoadTexture(globalRenderer, path.c_str());
	if (texture == nullptr) {
		std::cout << "Failed to load texture from path: " << path << "!";
	}

	return texture;
}



void Manager::run() {
	initSystems();
	loadGame();
	std::vector<Circle> circles;
	

	srand(time(NULL));

	for (int i = 0; i < 1; i++)
	{
		Circle *circle = new Circle(globalRenderer,		SCREEN_WIDTH/2 , SCREEN_HEIGHT/2, 40, 40);
		
		circles.push_back(*circle);
	}



	SDL_Event evnt;


	while (bIsRunning)
	{
		while (SDL_PollEvent(&evnt))
		{
			switch (evnt.type) {
			case SDL_QUIT:
				bIsRunning = false;
				break;

			case SDL_KEYDOWN:
				switch (evnt.key.keysym.scancode) {
					case SDL_SCANCODE_LEFT:
						circles[0].m_velX = -5;
						break;
					case SDL_SCANCODE_RIGHT:
						circles[0].m_velX = 5;
						break;

					case SDL_SCANCODE_SPACE:
						circles[0].m_velY = -10;
						break;

				}
				break;
			}
			
		}

		SDL_SetRenderDrawColor(globalRenderer, 255, 255, 255, 255);
		SDL_RenderClear(globalRenderer);

		for (int i = 0; i < circles.size(); i++)
		{
			circles[i].move();


			float distance;
			float radiDistance;

			for (int j = 0; j < circles.size(); j++)
			{
				

				distance = sqrt(pow(circles[i].m_xPos - circles[j].m_xPos, 2) + pow(circles[i].m_yPos - circles[j].m_yPos, 2));
				radiDistance = circles[i].m_radi + circles[j].m_radi;

				if (distance < radiDistance) {

					float newVelX1 = (circles[i].m_velX * (circles[i].m_radi - circles[j].m_radi) + (2 * circles[j].m_radi * circles[j].m_velX)) / (circles[i].m_radi + circles[j].m_radi);
					float newVelY1 = (circles[i].m_velY * (circles[i].m_radi - circles[j].m_radi) + (2 * circles[j].m_radi * circles[j].m_velY)) / (circles[i].m_radi + circles[j].m_radi);
					float newVelX2 = (circles[j].m_velX * (circles[j].m_radi - circles[i].m_radi) + (2 * circles[i].m_radi * circles[i].m_velX)) / (circles[j].m_radi + circles[i].m_radi);
					float newVelY2 = (circles[j].m_velY * (circles[j].m_radi - circles[i].m_radi) + (2 * circles[i].m_radi * circles[i].m_velY)) / (circles[j].m_radi + circles[i].m_radi);


					circles[i].m_velX = newVelX1;
					circles[i].m_velY = newVelY1;

					circles[j].m_velX = newVelX2;
					circles[j].m_velY = newVelY2;

				}

			}
		}
	

		for (int i = 0; i < circles.size(); i++)
		{
			circles[i].drawCircle();
		}

		SDL_RenderPresent(globalRenderer);

	}
}
