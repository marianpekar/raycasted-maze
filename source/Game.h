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

	float GetStartX() const { return m_startX; }
	float GetStartY() const { return m_startY; }

	std::shared_ptr<Player> GetPlayer() { return m_player; }
	std::shared_ptr<Surfaces> GetSurfaces() { return m_surfaces; }
	std::shared_ptr<Node> GetCurrentNode() { return m_currentNode; }
	std::shared_ptr<Maze> GetMaze() { return m_maze; }
	const Ray* GetRays() const { return m_rays; }
	
	bool IsRunning() const { return m_isGameRunning; }
private:
	float m_startX = 0, m_startY = 0;
	
	std::shared_ptr<Player> m_player;
	std::shared_ptr<Maze> m_maze;
	std::shared_ptr<Surfaces> m_surfaces;
	std::shared_ptr<Raycaster> m_raycaster;
	Ray m_rays[NUM_RAYS] = {};

	std::shared_ptr<Node> m_currentNode = nullptr;
	
	bool m_isGameRunning = false;
	int m_ticksLastFrame = 0;
	
	void SimulateMovement(float deltaTime);
	void GatherInput();
	void MovePlayer(float deltaTime) const;
};

