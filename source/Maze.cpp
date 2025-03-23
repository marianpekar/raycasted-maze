#include <iostream>
#include "Maze.h"
#include "PerlinNoise.h"

void Maze::TileFromPosition(const float x, const float y, int& tileX, int& tileY)
{
    tileX = static_cast<int>(std::floor(x / TILE_SIZE));
    tileY = static_cast<int>(std::floor(y / TILE_SIZE));
}

Maze::Maze(int startX, int startY) : m_startX(startX), m_startY(startY)
{
    m_perlinNoise = std::make_shared<PerlinNoise>(false);
    GenerateMaze();
}

int Maze::HasWallAt(const float x, const float y) const
{
    if (x < 0 || x > MAZE_WIDTH * TILE_SIZE || y < 0 || y > MAZE_HEIGHT * TILE_SIZE)
    {
        return true;
    }

    const int tileX = static_cast<int>(std::floor(x / TILE_SIZE));
    const int tileY = static_cast<int>(std::floor(y / TILE_SIZE));
    return GetAt(tileX, tileY) != 0;
}

void Maze::GetRandomTile(float& x, float& y)
{
    x = 1.f + static_cast<float>(std::rand() % (MAZE_HEIGHT - 2));
    y = 1.f + static_cast<float>(std::rand() % (MAZE_WIDTH - 2));
}

void Maze::GetRandomOpenLocation(float& x, float& y) const
{
    do
    {
        x = 1.f + static_cast<float>(std::rand() % (MAZE_HEIGHT - 2)) * TILE_SIZE + 1.f;
        y = 1.f + static_cast<float>(std::rand() % (MAZE_WIDTH - 2)) * TILE_SIZE + 1.f;
    }
    while (HasWallAt(x, y));
}

int Maze::GetAt(const int x, const int y) const
{
    return data[x + y * MAZE_HEIGHT];
}

void Maze::SetAt(const int x, const int y, const int val)
{
    data[x + y * MAZE_HEIGHT] = val;
}

void Maze::GenerateMaze()
{
    for (auto x = 0; x < MAZE_WIDTH; x++)
    {
        for (auto y = 0; y < MAZE_HEIGHT; y++)
        {
            SetAt(x, y, (x == m_startX && y == m_startY) ? 0 : 1);
        }
    }

    Dig(m_startX, m_startY);
    PaintWalls();
}

void Maze::Dig(int y, int x)
{
    int directions[4]{};
    for (int& direction : directions)
    {
        direction = std::rand() % 4;
    }

    for (int direction : directions)
    {
        switch (direction)
        {
        case 0: // Up
            if (y - 2 <= 0)
                continue;

            if (GetAt(x, y - 2) != 0)
            {
                SetAt(x, y - 2, 0);
                SetAt(x, y - 1, 0);
                Dig(y - 2, x);
            }
            break;

        case 1: // Right
            if (x + 2 >= MAZE_WIDTH - 1)
                continue;

            if (GetAt(x + 2, y) != 0)
            {
                SetAt(x + 2, y, 0);
                SetAt(x + 1, y, 0);
                Dig(y, x + 2);
            }
            break;

        case 2: // Down
            if (y + 2 >= MAZE_HEIGHT - 1)
                continue;

            if (GetAt(x, y + 2) != 0)
            {
                SetAt(x, y + 2, 0);
                SetAt(x, y + 1, 0);
                Dig(y + 2, x);
            }
            break;

        case 3: // Left
            if (x - 2 <= 0)
                continue;

            if (GetAt(x - 2, y) != 0)
            {
                SetAt(x - 2, y, 0);
                SetAt(x - 1, y, 0);
                Dig(y, x - 2);
            }
            break;
        default: ;
        }
    }
}

bool Maze::IsValid(const int x, const int y)
{
    return x >= 0 &&
           x <= MAZE_WIDTH * static_cast<int>(TILE_SIZE) &&
           y >= 0 &&
           y <= MAZE_HEIGHT * static_cast<int>(TILE_SIZE);
}

void Maze::PaintWalls()
{
    for (auto x = 0; x < MAZE_WIDTH; x++)
    {
        for (auto y = 0; y < MAZE_HEIGHT; y++)
        {
            if (GetAt(x, y) != 0)
                PaintWall(x, y);
        }
    }
}

void Maze::PaintWall(const int x, const int y)
{
    auto noiseSample = m_perlinNoise->FractalBrownianMotion(
        static_cast<float>(y) * FBM_X_SCALE,
        static_cast<float>(x) * FBM_Y_SCALE,
        FBM_OCTAVES,
        FBM_PERSISTENCE
    );

    auto noiseRemapped = m_perlinNoise->Remap(noiseSample, -0.33f, 0.33f, 1.0f, NUM_TEXTURES + 1);
    auto textureId = static_cast<int>(floor(noiseRemapped));

    SetAt(x, y, textureId);
}
