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
		globalWindow = SDL_CreateWindow("Elastic collision", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
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

	SDL_Rect rect; 
	SDL_Rect rect2;
	float velX1 = 4;
	float velX2 = -4;

	
	
	float distance;
	float radiDistance; 
	srand(time(NULL));
	bool collision; 
	//creates the circles
	for (int i = 0; i < 10; i++)
	{
		collision = false;
		Circle *circle = new Circle(globalRenderer,rand() % (SCREEN_WIDTH - 40)+ 30, rand() % (SCREEN_HEIGHT - 90) + 30, rand() % 20 + 10, rand() % 360);
		
		for (int i = 0; i < circles.size(); i++) {
			//the distance between the center points of the circles
			distance = sqrt(pow(circle->m_xPos - circles[i].m_xPos, 2) + pow(circle->m_yPos - circles[i].m_yPos, 2));

			//the lowest possible distance before collision between the ball
			radiDistance = circle->m_radi + circles[i].m_radi;
			if (distance <= radiDistance)
				collision = true; 
		}
		if(!collision)
			circles.push_back(*circle);
		
	}

	rect.w = 60;
	rect.h = 50;
	rect.x = SCREEN_WIDTH / 2;
	rect.y = SCREEN_HEIGHT - rect.h;

	rect2.w = 97.5;
	rect2.h = 50;
	rect2.x = 0;
	rect2.y = SCREEN_HEIGHT - rect.h;




	SDL_Event evnt;


	while (bIsRunning)
	{
		while (SDL_PollEvent(&evnt))
		{
			switch (evnt.type) {
				//when the x button in the top right corner is pressed, the game closes
			case SDL_QUIT:
				bIsRunning = false;
				break;

				//makes it possible to move the ball to the left / right and jump
			
			}
			
		}

		rect.x += velX1;
		rect2.x += velX2; 

		if (rect.x < 0)
			velX1 *= -1; 
		if(rect.x + rect.w > SCREEN_WIDTH)
			velX1 *= -1;
		if (rect2.x < 0)
			velX2 *= -1;
		if (rect2.x + rect.w > SCREEN_WIDTH)
			velX2 *= -1;


		//sets the draw color to white
		SDL_SetRenderDrawColor(globalRenderer, 255, 255, 255, 255);
		//clear the renderer with the set draw color 
		SDL_RenderClear(globalRenderer);
		

		if (!(rect.x + rect.w < rect2.x || rect.x > rect2.x + rect2.w)) {
			float newVelX1 = (velX1 * (rect.w - rect2.w) + (2 * rect2.w * velX2)) / (rect.w + rect2.w);
			float newVelX2 = (velX2 * (rect2.w - rect.w) + (2 * rect.w * velX1)) / (rect.w + rect2.w);

			velX1 = newVelX1;
			velX2 = newVelX2; 
		}
		
		SDL_SetRenderDrawColor(globalRenderer, 255, 0, 0, 255);

		SDL_RenderFillRect(globalRenderer, &rect);

		SDL_SetRenderDrawColor(globalRenderer, 0, 0, 255, 255);
		SDL_RenderFillRect(globalRenderer, &rect2);

		SDL_SetRenderDrawColor(globalRenderer, 0, 255, 0, 255);

		SDL_RenderDrawLine(globalRenderer, 0, SCREEN_HEIGHT -50, SCREEN_WIDTH, SCREEN_HEIGHT - 50); 

		SDL_RenderCopy(globalRenderer, nullptr, &rect, NULL);
		SDL_RenderCopy(globalRenderer, nullptr, &rect2, NULL);

		//  circle collision with circle
		for (int i = 0; i < circles.size(); i++)
		{
			//moves every circle
			circles[i].move();

			for (int k = 0; k < circles.size(); k++)
			{
				Circle *circle = &circles[k];

				if (checkCollision(rect, circle))
				{

					//collision from above
					if (circle->m_yPos < rect.y)
					{
						circle->m_yPos = rect.y - circle->m_radi;
						circle->m_velY = 0;
					}

					//collision from the left
					else if (circle->m_xPos < rect.x)
					{
						circle->m_velX = -circle->m_velX;
					}

					//collisionfrom the right
					else if (circle->m_xPos > rect.x + rect.w)
					{
						circle->m_velX = -circle->m_velX;
					}

				
				}
				
			}


			float distance;
			float radiDistance;

			for (int j = 0; j < circles.size(); j++)
			{
				
				//the distance between the center points of the circles
				distance = sqrt(pow(circles[i].m_xPos - circles[j].m_xPos, 2) + pow(circles[i].m_yPos - circles[j].m_yPos, 2));

				//the lowest possible distance before collision between the ball
				radiDistance = circles[i].m_radi + circles[j].m_radi;

				if (distance <= radiDistance) {

					//calculation of elastic collision for x and y 
					float newVelX1 = (circles[i].m_velX * (circles[i].m_radi - circles[j].m_radi) + (2 * circles[j].m_radi * circles[j].m_velX)) / (circles[i].m_radi + circles[j].m_radi);
					float newVelY1 = (circles[i].m_velY * (circles[i].m_radi - circles[j].m_radi) + (2 * circles[j].m_radi * circles[j].m_velY)) / (circles[i].m_radi + circles[j].m_radi);
					float newVelX2 = (circles[j].m_velX * (circles[j].m_radi - circles[i].m_radi) + (2 * circles[i].m_radi * circles[i].m_velX)) / (circles[j].m_radi + circles[i].m_radi);
					float newVelY2 = (circles[j].m_velY * (circles[j].m_radi - circles[i].m_radi) + (2 * circles[i].m_radi * circles[i].m_velY)) / (circles[j].m_radi + circles[i].m_radi);

					//applies the new x and y velocities for circle 1
					circles[i].m_velX = newVelX1;
					circles[i].m_velY = newVelY1;

					//applies the new x and y velocities for circle 2
					circles[j].m_velX = newVelX2;
					circles[j].m_velY = newVelY2;

				}

			}
		}
	
		//draws every circle to the screen
		for (int i = 0; i < circles.size(); i++)
		{
			circles[i].drawCircle();
		}

		//presents the renderer with all rendered obejcts
		SDL_RenderPresent(globalRenderer);

	}
}


bool Manager::checkCollision(SDL_Rect rect, Circle *circle)
{
	int cX, cY;

	//find the closest x offset
	if (circle->m_xPos + circle->m_velX< rect.x)
		cX = rect.x;
	else if (circle->m_xPos + circle->m_velX > rect.x + rect.w)
		cX = rect.x + rect.w;
	else
		cX = circle->m_xPos;

	//fins the closest y offset
	if (circle->m_yPos + circle->m_velY < rect.y)
		cY = rect.y;

	else if (circle->m_yPos + circle->m_velY> rect.y + rect.h)
		cY = rect.y + rect.h;
	else
		cY = circle->m_yPos;

	//if the closest point is inside the circle
	if (((cX - circle->m_xPos) * (cX - circle->m_xPos) + (cY - circle->m_yPos) * (cY - circle->m_yPos)) < circle->m_radi * circle->m_radi)
	{
		//This rect and the circle have collided
		return true;
	}

	//if the shaes have no collided
	return false;
}