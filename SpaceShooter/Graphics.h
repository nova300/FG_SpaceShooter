#pragma once

#include <vector>
#include <set>
#include <SDL.h>
#include <SDL_image.h>
#include <hlslpp/hlsl++.h>

class Sprite
{
public:
	Sprite(hlslpp::float2 pos = hlslpp::float2(0.0f, 0.0f), int textureIndex = 0);
	~Sprite();

	void Render();

	void Set(hlslpp::float2 pos, double angle = 0.0, int textureIndex = -1, SDL_Point center = {33, 33}, SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
	SDL_Rect renderQuad;
	SDL_Rect clip;
	double angle;
	int textureIdx;
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

const int screenX = 1024 - 64;
const int screenY = 768 - 64;