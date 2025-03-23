#pragma once

#include "SDL_image.h"
#include "Constants.h"

class Surfaces
{
public:
	Surfaces();
	uint32_t* m_textures[NUM_TEXTURES + 1];
	uint32_t m_minimapColors[NUM_TEXTURES + 1];
	static uint32_t GetPixel(const uint32_t* texels, int x, int y);
	static uint32_t Darken(uint32_t color, float amount);
private:
	static uint32_t* LoadPixels(const char* filePath);
};

