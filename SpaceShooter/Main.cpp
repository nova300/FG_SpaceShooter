#include "Graphics.h"
#include "Physics.h"
#include <stdio.h>

using hlslpp::float2;

bool run;

float deltaTime = 1.0f;
float time;

bool leftHeld = false;
bool rightHeld = false;
bool upHeld = false;

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
			
		case SDLK_DOWN:
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

		case SDLK_DOWN:
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

	Sprite s;
	VelocityMovement vm;
	hlslpp::float2 pos = hlslpp::float2(1.0f, 1.0f);
	double angle = -0.25;

	//vm.AddVector(hlslpp::float2(0.0002f, 0.0002f));

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
			handleInput(e);
			if (e.type == SDL_QUIT)
			{
				run = false;
			}

		}
		if (leftHeld)
		{
			angle -= 0.001 * deltaTime;
		}
		if (rightHeld)
		{
			angle += 0.001 * deltaTime;
		}
		if (upHeld)
		{

			float2 fireVec = float2(cos(angle * 6.28), sin(angle * 6.28));

			fireVec = hlslpp::normalize(fireVec);

			fireVec /= 1000;

			fireVec *= (0.005f * deltaTime);

			vm.AddVector(fireVec);
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
		
		s.Set(pos, angle);

		pos = vm.Update(pos, deltaTime);

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