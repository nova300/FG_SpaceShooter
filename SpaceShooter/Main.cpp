#include "Graphics.h"
#include <stdio.h>

bool run;

float deltaTime = 1.0f;
float time;

SDL_Window* window;

bool init()
{
	bool ok = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		ok = false;
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags) && ok)
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		ok = false;
	}

	if (ok)
	{
		window = SDL_CreateWindow("Space War", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI);

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		ok = renderer;
	}

	return ok;
}

int main(int argumentCount, char * arguments[])
{
	if (!init())
	{
		printf("init failed :c\n");
		return EXIT_FAILURE;
	}

	if (!LoadSpriteAtlas("testatlas.png"))
	{
		return EXIT_FAILURE;
	}

	Sprite s;

	s.Set(16, 16, 2);

	int counter = 0;
	float timer = 0.0f;
	
	SDL_Event e;
	run = true;
	while (run)
	{
		deltaTime = SDL_GetTicks() - time;
		if (deltaTime > 250) deltaTime = 250;
		time = SDL_GetTicks();

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				run = false;
			}

		}

		//prerender section
		
		if (timer > 1000.0f)
		{
			counter++;
			timer = 0.0f;
		}
		else
		{
			timer = timer + deltaTime;
		}
		
		s.Set((int)(time / 4) % 1024, (int)(time / 4) % 768, counter, time / 2);

		SDL_SetRenderDrawColor(renderer, 0x00, 0xA0, 0xA0, 0xFF);
		SDL_RenderClear(renderer);

		//render section
		for (Sprite* s : Sprites)
		{
			s->Render();
		}

		SDL_RenderPresent(renderer);
	}

	

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;

	IMG_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}