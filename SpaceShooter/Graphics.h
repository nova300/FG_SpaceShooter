#pragma once

#include <vector>
#include <set>
#include <SDL.h>
#include <SDL_image.h>
#include <hlslpp/hlsl++.h>

class Sprite
{
public:

	Sprite(int x = 0, int y = 0);
	~Sprite();

	void Render();

	void Set(int x, int y, int textureIndex = 0, double angle = 0.0, SDL_Point center = {32, 32}, SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
	SDL_Rect renderQuad;
	SDL_Rect clip;
	double angle;
	SDL_Point center; 
	SDL_RendererFlip flip;

};

extern std::set<Sprite*> Sprites;

extern SDL_Renderer *renderer;

extern SDL_Texture* texture;

extern bool run;

extern float deltaTime;

bool LoadSpriteAtlas(const char* path);
extern int w;
extern int h;
extern int hsize;
extern int vsize;