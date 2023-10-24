#pragma once

#include <vector>
#include <set>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <hlslpp/hlsl++.h>

class RenderObject
{
public:
	RenderObject();
	~RenderObject();

	virtual void Render() = 0;

	bool Hide;
};

class Sprite : public RenderObject
{
public:
	void Render();

	void Set(hlslpp::float2 pos = hlslpp::float2(1.0, 1.0), double angle = 0.0, int textureIndex = -1, SDL_Point center = {32, 32}, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void Set(int textureIndex);

	int textureIdx;

private:
	SDL_Rect renderQuad;
	SDL_Rect clip;
	double angle;
	SDL_Point center; 
	SDL_RendererFlip flip;

};

class Line : RenderObject
{
public:
	void Render();

	void Set(hlslpp::float2 pos, hlslpp::float2 end);

private:
	int ax;
	int ay;
	int bx;
	int by;
	char r;
	char g;
	char b;
};

class Text : public RenderObject
{
public:
	Text();
	~Text();

	void Render();

	void Set(hlslpp::float2 pos);
	void Set(const char* text);

private:
	SDL_Texture* fontTexture;
	SDL_Rect renderQuad;
	int width;
	int height;
};

extern std::set<RenderObject*> RenderObjects;

extern SDL_Renderer *renderer;

extern SDL_Texture* texture;

extern TTF_Font* font;

extern bool run;

extern float deltaTime;

bool LoadSpriteAtlas(const char* path);
extern int w;
extern int h;
extern int hsize;
extern int vsize;

const int screenX = 1024 - 64;
const int screenY = 768 - 64;