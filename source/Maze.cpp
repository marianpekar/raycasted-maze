#include "Maze.h"

Maze::Maze()
{
}

Maze::Maze(int startX, int startY) : m_startX(startX), m_startY(startY)
{
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

void Maze::GenerateMaze()
{
    int x, y;

    for (x = 0; x < MAZE_NUM_COLS; x++) {
        for (y = 0; y < MAZE_NUM_ROWS; y++) {
            m_maze[x][y] = (x == m_startX && y == m_startY) ? 0 : 1;
        }
    }

    Dig(m_startX, m_startY);
}

void Maze::Dig(int row, int column)
{
    int directions[4];
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
