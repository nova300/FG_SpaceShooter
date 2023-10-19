#pragma once

#include <vector>
#include <set>
#include <SDL.h>
#include <SDL_image.h>
#include <hlslpp/hlsl++.h>

class RenderObject
{
public:
	RenderObject(bool addToQueue = true);
	~RenderObject();

	virtual void Render() = 0;
private:
	bool registered;
};

class Sprite : RenderObject
{
public:
	void Render();

	void Set(hlslpp::float2 pos = hlslpp::float2(1.0, 1.0), double angle = 0.0, int textureIndex = 0, SDL_Point center = {32, 32}, SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
	SDL_Rect renderQuad;
	SDL_Rect clip;
	double angle;
	int textureIdx;
	SDL_Point center; 
	SDL_RendererFlip flip;

};

class Line : RenderObject
{
public:
	void Render();

	void Set(hlslpp::float2 pos, double angle);

private:
	int ax;
	int ay;
	int bx;
	int by;
	char r;
	char g;
	char b;
};

extern std::set<RenderObject*> RenderObjects;

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