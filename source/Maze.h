#pragma once
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "PerlinNoise.h"
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
	PerlinNoise m_perlinNoise = PerlinNoise(false);
	void GenerateMaze();
	void Dig(int row, int column);
	void PaintWalls();
	void PaintWall(int row, int column);
	void PrintMazeToConsole(); // just for debugging
};

