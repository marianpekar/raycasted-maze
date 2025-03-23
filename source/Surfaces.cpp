#include "Surfaces.h"

Surfaces::Surfaces()
{
    m_textures[1] = LoadPixels(REDBRICK_TEXTURE_FILEPATH);
    m_textures[2] = LoadPixels(PURPLESTONE_TEXTURE_FILEPATH);
    m_textures[3] = LoadPixels(MOSSYSTONE_TEXTURE_FILEPATH);
    m_textures[4] = LoadPixels(GRAYSTONE_TEXTURE_FILEPATH);
    m_textures[5] = LoadPixels(COLORSTONE_TEXTURE_FILEPATH);
    m_textures[6] = LoadPixels(BLUESTONE_TEXTURE_FILEPATH);
    m_textures[7] = LoadPixels(WOOD_TEXTURE_FILEPATH);
    m_textures[8] = LoadPixels(EAGLE_TEXTURE_FILEPATH);

    m_minimapColors[0] = 0xFF000000;
    m_minimapColors[1] = REDBRICK_TEXTURE_MINIMAP_COLOR;
    m_minimapColors[2] = PURPLESTONE_TEXTURE_MINIMAP_COLOR;
    m_minimapColors[3] = MOSSYSTONE_TEXTURE_MINIMAP_COLOR;
    m_minimapColors[4] = GRAYSTONE_TEXTURE_MINIMAP_COLOR;
    m_minimapColors[5] = COLORSTONE_TEXTURE_MINIMAP_COLOR;
    m_minimapColors[6] = BLUESTONE_TEXTURE_MINIMAP_COLOR;
    m_minimapColors[7] = WOOD_TEXTURE_MINIMAP_COLOR;
    m_minimapColors[8] = EAGLE_TEXTURE_MINIMAP_COLOR;
}

uint32_t Surfaces::GetPixel(const uint32_t* texels, int x, int y)
{
    return texels[y * TEXTURE_WIDTH + x];
}

uint32_t Surfaces::Darken(uint32_t color, float amount)
{
    amount = 1.0f - amount;
    const uint32_t a = color >> 24 & 0xFF;
    const uint32_t r = static_cast<uint32_t>(static_cast<float>((color >> 16 & 0xFF)) * amount);
    const uint32_t g = static_cast<uint32_t>(static_cast<float>(color >> 8 & 0xFF) * amount);
    const uint32_t b = static_cast<uint32_t>(static_cast<float>(color & 0xFF) * amount);

    return (a << 24) | (r << 16) | (g << 8) | b;
}

uint32_t* Surfaces::LoadPixels(const char* filePath)
{
    const SDL_Surface* image = IMG_Load(filePath);
    uint32_t* pixels = static_cast<uint32_t*>(image->pixels);

    return pixels;
}
