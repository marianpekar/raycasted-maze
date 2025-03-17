#pragma once

#include "Constants.h"

class Maze
{
public:
	Maze();
	~Maze();
	bool IsValid(int x, int y);
	void TileFromPosition(float x, float y, int& tileX, int& tileY) const;
	Maze(int startX, int startY);
	int m_maze[MAZE_NUM_COLS][MAZE_NUM_ROWS];
	int HasWallAt(float x, float y);
	static void GetRandomTile(float& x, float& y);
	void GetRandomOpenLocation(float& x, float& y);
private:
	int m_startX;
	int m_startY;
	class PerlinNoise* m_perlinNoise;
	void GenerateMaze();
	void Dig(int row, int column);
	void PaintWalls();
	void PaintWall(int row, int column);
	void PrintMazeToConsole(); // just for debugging
};

