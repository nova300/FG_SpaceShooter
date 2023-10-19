#include "Graphics.h"
#include "Physics.h"
#include "Game.h"
#include <stdio.h>
#include <queue>

using hlslpp::float2;
using hlslpp::float3;

bool run;

float deltaTime = 1.0f;
float time;

bool leftHeld = false;
bool rightHeld = false;
bool upHeld = false;
bool spaceHeld = false;

SDL_Window* window;

void handleInput(SDL_Event event)
{
	if (event.type == SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_UP:
			upHeld = true;
			break;
			
		case SDLK_SPACE:
			spaceHeld = true;
			break;

		case SDLK_LEFT:
			leftHeld = true;
			break;

		case SDLK_RIGHT:
			rightHeld = true;
			break;

		default:
			break;
		}

	}
	if (event.type == SDL_KEYUP)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_UP:
			upHeld = false;
			break;

		case SDLK_SPACE:
			spaceHeld = false;
			break;

		case SDLK_LEFT:
			leftHeld = false;
			break;

		case SDLK_RIGHT:
			rightHeld = false;
			break;

		default:
			break;
		}

	}


}

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

	PlayerShip player;
	
	SDL_Event e;
	run = true;
	while (run)
	{
		deltaTime = SDL_GetTicks() - time;
		if (deltaTime > 250) deltaTime = 250;
		time = SDL_GetTicks();

		while (SDL_PollEvent(&e) != 0)
		{
			handleInput(e);
			if (e.type == SDL_QUIT)
			{
				run = false;
			}

		}
		if (leftHeld)
		{
			player.angle -= 0.001 * deltaTime;
		}
		if (rightHeld)
		{
			player.angle += 0.001 * deltaTime;
		}
		if (upHeld)
		{
			player.Thruster();
		}
		if (spaceHeld)
		{
			player.Fire();
		}

		//prerender section
		
		player.Update(deltaTime);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(renderer);

		//render section
		for (RenderObject* s : RenderObjects)
		{
			s->Render();
		}

		player.sprite.Render();

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