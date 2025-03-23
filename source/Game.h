#pragma once

#include <memory>

#include "Constants.h"
#include "Ray.h"

struct Node;
class Player;
class Maze;
class Surfaces;
class Raycaster;

class Game
{
public:
	void Setup(unsigned int seed, bool isBot);
	void Update();
	void Delay(float time);
	
	float m_startX;
	float m_startY;
	
	std::shared_ptr<Player> m_player;
	std::shared_ptr<Maze> m_maze;
	std::shared_ptr<Surfaces> m_surfaces;
	std::shared_ptr<Raycaster> m_raycaster;
	Ray m_rays[NUM_RAYS];

	std::shared_ptr<Node> m_currentNode = nullptr;
	
	bool m_isGameRunning;
	int m_ticksLastFrame;
private:
	void SimulateMovement(float deltaTime);
	void GatherInput();
	void MovePlayer(float deltaTime) const;
};

