#include "GameManager.h"
#include <Windows.h>
#include <SDL\SDL.h>
#include <SDL\SDL_image.h>
#include <SDL\SDL_ttf.h>
#include <SDL\SDL_mixer.h>
#include <iostream>
#include <string>
#include "Player.h"

int main(int argc, char *argv[])
{
	
	//Initializing and loading variables
	SDL_Window *window = nullptr;
	SDL_Renderer *renderTarget = nullptr;

	const Uint8 *KEY_STATE;

	float frameTime = 0;
	int prevTime = 0;
	int currentTime = 0;
	float deltaTime = 0,

	SDL_Init(SDL_INIT_VIDEO);

	if (TTF_Init() < 0)
		std::cout << "Error: " << TTF_GetError() << std::endl;

	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;

	if (!(IMG_Init(IMG_INIT_PNG) && imgFlags))
		std::cout << "Error: " << IMG_GetError();

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		std::cout << "Error:" << Mix_GetError() << std::endl;

	//Longer files like background music (+10 secs)
	Mix_Music *bgm = Mix_LoadMUS("Spectre.mp3");
	//Shorter files
	//Mix_Chunk *soundEffect = Mix_LoadWAV("");

	window = SDL_CreateWindow("SDL GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_RESIZABLE);
	renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	Player player1(renderTarget, "doctorSS.png", 0, 0, 4, 4);

	TTF_Font *font = TTF_OpenFont("sample.ttf", 20);
	SDL_Color color = { 234,141,255,255 };
	SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Random text", color);
	SDL_Texture *text = SDL_CreateTextureFromSurface(renderTarget, textSurface);
	SDL_Rect textRect;
	textRect.x = textRect.y = 0;
	SDL_QueryTexture(text, NULL, NULL, &textRect.w, &textRect.h);

	SDL_FreeSurface(textSurface);
	textSurface = nullptr;

	bool isRunning = true;
	SDL_Event evnt;

	while (isRunning)
	{
		std::cout << "TEST" << std::endl;
		prevTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = (currentTime - prevTime) / 1000.0f;

		while (SDL_PollEvent(&evnt))
		{
			if (evnt.type == SDL_QUIT)
				isRunning = false;

			else if (evnt.type == SDL_KEYDOWN)
			{
				switch (evnt.key.keysym.sym)
				{
				case SDLK_p:
					if (!Mix_PlayingMusic())
						Mix_PlayMusic(bgm, -1);
					else if (Mix_PausedMusic())
						Mix_ResumeMusic();
					else
						Mix_PauseMusic();
					break;

				case SDLK_s:
					Mix_HaltMusic();
					break;

				
				}
			}
		}
		KEY_STATE = SDL_GetKeyboardState(NULL);
		
		player1.update(deltaTime, KEY_STATE);
	
		SDL_SetRenderDrawColor(renderTarget, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderTarget);

		player1.draw(renderTarget);
		
		

		SDL_RenderPresent(renderTarget);
	}

	SDL_DestroyRenderer(renderTarget);
	SDL_DestroyTexture(text);
	SDL_DestroyWindow(window);
	//Mix_FreeChunk(soundEffect);
	Mix_FreeMusic(bgm);

	//soundEffect = nullptr;
	bgm = nullptr;
	window = nullptr;
	renderTarget = nullptr;
	text = nullptr;

	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}