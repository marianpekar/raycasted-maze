#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Maze.h"
#include "PerlinNoise.h"

Maze::~Maze()
{
    delete m_perlinNoise;
}

bool Maze::IsValid(int x, int y)
{
    return x >= 0 && x <= MAZE_NUM_COLS * TILE_SIZE && y >= 0 && y <= MAZE_NUM_ROWS * TILE_SIZE;
}

void Maze::TileFromPosition(float x, float y, int& tileX, int& tileY) const
{
    tileX = floor(x / TILE_SIZE);
    tileY = floor(y / TILE_SIZE);
}

Maze::Maze(int startX, int startY) : m_startX(startX), m_startY(startY)
{
    m_perlinNoise = new PerlinNoise(false);
	GenerateMaze();
}

int Maze::HasWallAt(float x, float y)
{
    if (x < 0 || x > MAZE_NUM_COLS * TILE_SIZE || y < 0 || y > MAZE_NUM_ROWS * TILE_SIZE) {
        return true;
    }
    
    const int mapGridIndexX = floor(x / TILE_SIZE);
    const int mapGridIndexY = floor(y / TILE_SIZE);
    return m_maze[mapGridIndexY][mapGridIndexX] != 0;
}

void Maze::GetRandomTile(float& x, float& y)
{
    x = 1 + rand() % (MAZE_NUM_ROWS - 2);
    y = 1 + rand() % (MAZE_NUM_COLS - 2);
}

void Maze::GetRandomOpenLocation(float& x, float& y)
{
    do {
        x = 1 + rand() % (MAZE_NUM_ROWS - 2) * TILE_SIZE + 1;
        y = 1 + rand() % (MAZE_NUM_COLS - 2) * TILE_SIZE + 1;
    }
    while (HasWallAt(x, y));
}


void Maze::GenerateMaze()
{
	for (auto x = 0; x < MAZE_NUM_COLS; x++) {
        for (auto y = 0; y < MAZE_NUM_ROWS; y++) {
            m_maze[x][y] = (x == m_startX && y == m_startY) ? 0 : 1;
        }
    }

    Dig(m_startX, m_startY);
    PaintWalls();
}

void Maze::Dig(int row, int column)
{
    int directions[4]{};
    for (int d = 0; d < 4; d++) {
        directions[d] = rand() % 4;
    }

    for (int i = 0; i < 4; i++) {
        switch (directions[i])
        {
        case 0: // Up
            if (row - 2 <= 0)
                continue;

            if (m_maze[row - 2][column] != 0) {
                m_maze[row - 2][column] = 0;
                m_maze[row - 1][column] = 0;
                Dig(row - 2, column);
            }
            break;

        case 1: // Right
            if (column + 2 >= MAZE_NUM_COLS - 1)
                continue;

            if (m_maze[row][column + 2] != 0) {
                m_maze[row][column + 2] = 0;
                m_maze[row][column + 1] = 0;
                Dig(row, column + 2);
            }
            break;

        case 2: // Down
            if (row + 2 >= MAZE_NUM_ROWS - 1)
                continue;

            if (m_maze[row + 2][column] != 0) {
                m_maze[row + 2][column] = 0;
                m_maze[row + 1][column] = 0;
                Dig(row + 2, column);
            }
            break;

        case 3: // Left
            if (column - 2 <= 0)
                continue;

            if (m_maze[row][column - 2] != 0) {
                m_maze[row][column - 2] = 0;
                m_maze[row][column - 1] = 0;
                Dig(row, column - 2);
            }
            break;
        }
    }
}

void Maze::PaintWalls()
{
    for (auto x = 0; x < MAZE_NUM_COLS; x++) {
        for (auto y = 0; y < MAZE_NUM_ROWS; y++) {
            if (m_maze[x][y] != 0)
                PaintWall(x, y);      	
        }
    }

    PrintMazeToConsole();
}

void Maze::PaintWall(int row, int column)
{	
    auto noiseSample = m_perlinNoise->FractalBrownianMotion(row * FBM_X_SCALE, column * FBM_Y_SCALE, FBM_OCTAVES, FBM_PERSISTENCE);
    auto noiseRemaped = m_perlinNoise->Remap(noiseSample, -0.33f, 0.33f, 1.0f, NUM_TEXTURES + 1);
    auto textureId = static_cast<int>(floor(noiseRemaped));
	
    m_maze[row][column] = textureId;
}

void Maze::PrintMazeToConsole()
{
    for (auto x = 0; x < MAZE_NUM_COLS; x++) {
        for (auto y = 0; y < MAZE_NUM_ROWS; y++) {
            if (m_maze[x][y] == 0)
                std::cout << "  ";
            else
                std::cout << m_maze[x][y] << " ";
        }
        std::cout << std::endl;
    }
}
