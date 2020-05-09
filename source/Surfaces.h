#pragma once

#include "SDL_image.h"
#include "Constants.h"

class Surfaces
{
public:
	Surfaces();
	SDL_Surface* m_wallSurfaces[NUM_TEXTURES + 1];
	static uint32_t GetTexel(SDL_Surface* surface, int x, int y);
	static int Darken(int color, float amount);
private:
	static SDL_Surface* LoadSurface(const char* filePath);
};

