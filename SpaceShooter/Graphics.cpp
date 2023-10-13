#include "Graphics.h"

std::set<Sprite*> Sprites;

SDL_Texture* texture;

SDL_Renderer* renderer;

int w;
int h;
int hsize;
int vsize;

Sprite::Sprite(int x, int y)
{
	Sprites.insert(this);
	this->Set(x, y);
}

Sprite::~Sprite()
{
	Sprites.erase(this);
}

void Sprite::Render()
{
	SDL_RenderCopyEx(renderer, texture, &clip, &renderQuad, angle, &center, flip);
}

void Sprite::Set(int x, int y, int textureIndex, double angle, SDL_Point center, SDL_RendererFlip flip)
{
	int hpos = 64 * (int)(textureIndex % hsize);
	int vpos = 64 * (int)((textureIndex / hsize) % vsize);

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
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
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