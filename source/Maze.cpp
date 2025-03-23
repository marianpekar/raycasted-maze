#include <cstdlib>
#include <iostream>
#include "Maze.h"
#include "PerlinNoise.h"

bool Maze::IsValid(int x, int y)
{
    return x >= 0 && x <= MAZE_WIDTH * TILE_SIZE && y >= 0 && y <= MAZE_HEIGHT * TILE_SIZE;
}

void Maze::TileFromPosition(float x, float y, int& tileX, int& tileY) const
{
    tileX = std::floor(x / TILE_SIZE);
    tileY = std::floor(y / TILE_SIZE);
}

Maze::Maze(int startX, int startY) : m_startX(startX), m_startY(startY)
{
    m_perlinNoise = std::make_shared<PerlinNoise>(false);
	GenerateMaze();
}

int Maze::HasWallAt(float x, float y)
{
    if (x < 0 || x > MAZE_WIDTH * TILE_SIZE || y < 0 || y > MAZE_HEIGHT * TILE_SIZE) {
        return true;
    }
    
    const int mapGridIndexX = std::floor(x / TILE_SIZE);
    const int mapGridIndexY = std::floor(y / TILE_SIZE);
    return GetAt(mapGridIndexX, mapGridIndexY) != 0;
}

void Maze::GetRandomTile(float& x, float& y)
{
    x = 1 + std::rand() % (MAZE_HEIGHT - 2);
    y = 1 + std::rand() % (MAZE_WIDTH - 2);
}

void Maze::GetRandomOpenLocation(float& x, float& y)
{
    do {
        x = 1 + std::rand() % (MAZE_HEIGHT - 2) * TILE_SIZE + 1;
        y = 1 + std::rand() % (MAZE_WIDTH - 2) * TILE_SIZE + 1;
    }
    while (HasWallAt(x, y));
}

int Maze::GetAt(int x, int y) const
{
    return data[x + y * MAZE_HEIGHT];
}

void Maze::SetAt(int x, int y, int val)
{
    data[x + y * MAZE_HEIGHT] = val;
}

void Maze::GenerateMaze()
{
	for (auto x = 0; x < MAZE_WIDTH; x++) {
        for (auto y = 0; y < MAZE_HEIGHT; y++) {
            SetAt(x, y, (x == m_startX && y == m_startY) ? 0 : 1);
        }
    }

    Dig(m_startX, m_startY);
    PaintWalls();
}

void Maze::Dig(int y, int x)
{
    int directions[4]{};
    for (int d = 0; d < 4; d++) {
        directions[d] = std::rand() % 4;
    }

    for (int i = 0; i < 4; i++) {
        switch (directions[i])
        {
        case 0: // Up
            if (y - 2 <= 0)
                continue;

            if (GetAt(x, y - 2) != 0) {
                SetAt(x, y - 2, 0);
                SetAt(x, y - 1, 0);
                Dig(y - 2, x);
            }
            break;

        case 1: // Right
            if (x + 2 >= MAZE_WIDTH - 1)
                continue;

            if (GetAt(x + 2, y) != 0) {
                SetAt(x + 2, y, 0);
                SetAt(x + 1, y, 0);
                Dig(y, x + 2);
            }
            break;

        case 2: // Down
            if (y + 2 >= MAZE_HEIGHT - 1)
                continue;

            if (GetAt(x, y + 2) != 0) {
                SetAt(x, y + 2, 0);
                SetAt(x, y + 1, 0);
                Dig(y + 2, x);
            }
            break;

        case 3: // Left
            if (x - 2 <= 0)
                continue;

            if (GetAt(x - 2, y) != 0) {
                SetAt(x - 2, y, 0);
                SetAt(x - 1, y, 0);
                Dig(y, x - 2);
            }
            break;
        }
    }
}

void Maze::PaintWalls()
{
    for (auto x = 0; x < MAZE_WIDTH; x++) {
        for (auto y = 0; y < MAZE_HEIGHT; y++) {
            if (GetAt(x, y) != 0)
                PaintWall(x, y);      	
        }
    }
}

void Maze::PaintWall(int x, int y)
{	
    auto noiseSample = m_perlinNoise->FractalBrownianMotion(y * FBM_X_SCALE, x * FBM_Y_SCALE, FBM_OCTAVES, FBM_PERSISTENCE);
    auto noiseRemaped = m_perlinNoise->Remap(noiseSample, -0.33f, 0.33f, 1.0f, NUM_TEXTURES + 1);
    auto textureId = static_cast<int>(floor(noiseRemaped));

    SetAt(x, y, textureId);
}