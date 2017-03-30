/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include "Manager.h"

int main(int argc, char* args[]) {
	Manager game;
	game.run();

	return 0;
}