#include <SDL.h>
#include <hlslpp/hlsl++.h>

#include <iostream>

int main(int argumentCount, char * arguments[])
{
	SDL_Window * window = SDL_CreateWindow("SDL!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI);

	SDL_Delay(1000);

	SDL_DestroyWindow(window);

	return 0;
}