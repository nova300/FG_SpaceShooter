#include "Graphics.h"

using hlslpp::float2;

std::set<RenderObject*> RenderObjects;

SDL_Texture* texture;

SDL_Renderer* renderer;

int w;
int h;
int hsize;
int vsize;

RenderObject::RenderObject()
{
	RenderObjects.insert(this);
	Hide = false;
}

RenderObject::~RenderObject()
{
	RenderObjects.erase(this);
}

void Sprite::Render()
{
	if (!Hide) SDL_RenderCopyEx(renderer, texture, &clip, &renderQuad, ((angle + 0.25) * 6.28) * (180.0 / 3.14), &center, flip);
}

void Sprite::Set(hlslpp::float2 pos, double angle, int textureIndex, SDL_Point center, SDL_RendererFlip flip)
{
	if (textureIndex != -1)
	{
		textureIdx = textureIndex;
	}
	int hpos = 64 * (int)(textureIdx % hsize);
	int vpos = 64 * (int)((textureIdx / hsize) % vsize);

	int x = pos.x * (screenX / 2);
	int y = pos.y * (screenY / 2);

	//x = x % screenX;
	//y = y % screenY;

	this->clip = clip = { hpos, vpos, 64, 64};
	this->renderQuad = { x, y, 64, 64};
	this->angle = angle;
	this->center = center;
	
}

bool LoadSpriteAtlas(const char* path)
{
	w = 0;
	h = 0;

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface != NULL)
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0, 0));
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture != NULL)
		{
			w = loadedSurface->w;
			h = loadedSurface->h;
		}
		else printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());

		SDL_FreeSurface(loadedSurface);
	}
	else printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());

	texture = newTexture;
	hsize = (int)(w / 64);
	vsize = (int)(h / 64);

	return newTexture;
}

void Line::Render()
{
	SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
	SDL_RenderDrawLine(renderer, ax, ay, bx, by);
}

void Line::Set(hlslpp::float2 pos, hlslpp::float2 end)
{
	ax = pos.x * (screenX / 2) + 32;
	ay = pos.y * (screenY / 2) + 32;
	

	bx = end.x * (screenX / 2) + 32;
	by = end.y * (screenY / 2) + 32;

	r = rand() % 255;
	g = rand() % 255;
	b = rand() % 255;
}

Text::Text()
{
	fontTexture = NULL;
}

Text::~Text()
{
	if (fontTexture != NULL)
	{
		SDL_DestroyTexture(fontTexture);
		fontTexture = NULL;
		width = 0;
		height = 0;
	}
}

void Text::Set(const char* text)
{
	if (fontTexture != NULL)
	{
		SDL_DestroyTexture(fontTexture);
		fontTexture = NULL;
		width = 0;
		height = 0;
	}

	SDL_Color col = { 0xFF, 0xFF, 0xFF, 0xFF };

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, col);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		fontTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (fontTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			width = textSurface->w;
			height = textSurface->h;
			this->renderQuad = { renderQuad.x, renderQuad.y, width, height };
		}
		SDL_FreeSurface(textSurface);
	}
}

void Text::Set(float2 pos)
{
	int x = pos.x * (screenX / 2);
	int y = pos.y * (screenY / 2);

	this->renderQuad = { x, y, width, height};
}

void Text::Render()
{
	if (!Hide) SDL_RenderCopy(renderer, fontTexture, NULL, &renderQuad);
}