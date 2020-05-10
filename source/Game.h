#pragma once

#include <ctime>
#include "Maze.h"
#include "Player.h"
#include "Raycaster.h"
#include "Surfaces.h"

class Game
{
public:
	~Game();
	void Setup();
	void ProcessInput();
	void MovePlayer(float deltaTime);
	void Update();
	void Delay(float time);
	
	float m_startX;
	float m_startY;
	
	Player* m_player;
	Maze* m_maze;
	Surfaces* m_surfaces;
	Raycaster* m_raycaster;
	Ray m_rays[NUM_RAYS];

	bool m_isGameRunning;
	int m_ticksLastFrame;
};

