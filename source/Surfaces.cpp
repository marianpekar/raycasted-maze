#include "Surfaces.h"

Surfaces::Surfaces()
{
    m_wallSurfaces[1] = LoadSurface(REDBRICK_TEXTURE_FILEPATH);
    m_wallSurfaces[2] = LoadSurface(PURPLESTONE_TEXTURE_FILEPATH);
    m_wallSurfaces[3] = LoadSurface(MOSSYSTONE_TEXTURE_FILEPATH);
    m_wallSurfaces[4] = LoadSurface(GRAYSTONE_TEXTURE_FILEPATH);
    m_wallSurfaces[5] = LoadSurface(COLORSTONE_TEXTURE_FILEPATH);
    m_wallSurfaces[6] = LoadSurface(BLUESTONE_TEXTURE_FILEPATH);
    m_wallSurfaces[7] = LoadSurface(WOOD_TEXTURE_FILEPATH);
    m_wallSurfaces[8] = LoadSurface(EAGLE_TEXTURE_FILEPATH);
}

uint32_t Surfaces::GetTexel(SDL_Surface* surface, int x, int y)
{
	auto pixels = static_cast<uint32_t*>(surface->pixels);
    return pixels[(y * surface->w) + x];
}


int Surfaces::Darken(int color, float amount)
{
    amount = 1.0f - amount;
    auto a = (color >> 24) & 0xFF;
    auto r = static_cast<int>(((color >> 16) & 0xFF) * amount);
    auto g = static_cast<int>(((color >> 8) & 0xFF) * amount);
    auto b = static_cast<int>((color & 0xFF) * amount);

    return (a << 24) | (r << 16) | (g << 8) | b;
}

SDL_Surface* Surfaces::LoadSurface(const char* filePath)
{
    auto image = IMG_Load(filePath);
    return image;
}
