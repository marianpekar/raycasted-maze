#pragma once

#include <cmath>
#include "Constants.h"
#include "Player.h"
#include "Maze.h"
#include "Ray.h"

class Raycaster
{
public:
    void CastRays(Player* player, Maze* maze, Ray rays[NUM_RAYS]) const;
private:
    void CastRay(Player* player, Maze* maze, Ray rays[NUM_RAYS], float rayAngle, int stripId) const;
    float NormalizeAngle(float angle) const;
    float DistanceBetweenPoints(float x1, float y1, float x2, float y2) const;
};
