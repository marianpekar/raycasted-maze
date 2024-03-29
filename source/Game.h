#pragma once

#include "Constants.h"
#include "Ray.h"

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
	
	class Player* m_player;
	class Maze* m_maze;
	class Surfaces* m_surfaces;
	class Raycaster* m_raycaster;
	Ray m_rays[NUM_RAYS];

	bool m_isGameRunning;
	int m_ticksLastFrame;
};

