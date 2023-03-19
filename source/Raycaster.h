#pragma once

#include "Constants.h"

class Raycaster
{
public:
    void CastRays(class Player* player, class Maze* maze, struct Ray rays[NUM_RAYS]) const;
private:
    void CastRay(Player* player, Maze* maze, Ray rays[NUM_RAYS], float rayAngle, int stripId) const;
    float NormalizeAngle(float angle) const;
    float DistanceBetweenPoints(float x1, float y1, float x2, float y2) const;
};
