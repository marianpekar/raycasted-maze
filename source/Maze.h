#pragma once

#include <memory>

#include "Constants.h"

class PerlinNoise;

class Maze
{
public:
	bool IsValid(int x, int y);
	void TileFromPosition(float x, float y, int& tileX, int& tileY) const;
	Maze(int startX, int startY);
	int HasWallAt(float x, float y);
	static void GetRandomTile(float& x, float& y);
	void GetRandomOpenLocation(float& x, float& y);
	int GetAt(int x, int y) const;
	void SetAt(int x, int y, int val);
private:
	int data[MAZE_WIDTH * MAZE_HEIGHT];
	int m_startX, m_startY;
	std::shared_ptr<PerlinNoise> m_perlinNoise;
	void GenerateMaze();
	void Dig(int x, int y);
	void PaintWalls();
	void PaintWall(int x, int y);
};

