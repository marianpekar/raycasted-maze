#pragma once
#include <cstdlib>
#include "Constants.h"

class Maze
{
public:
	Maze();
	Maze(int startX, int startY);
	int m_maze[MAZE_NUM_COLS][MAZE_NUM_ROWS];
	int HasWallAt(float x, float y);
private:
	int m_startX;
	int m_startY;
	void GenerateMaze();
	void Dig(int row, int column);
};

