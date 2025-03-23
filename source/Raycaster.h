#pragma once

#include <memory>

#include "Constants.h"

struct Ray;
struct Player;
class Maze;

class Raycaster
{
public:
    void CastRays(const std::shared_ptr<Player>& player, const std::shared_ptr<Maze>& maze, Ray rays[NUM_RAYS]) const;
private:
    void CastRay(const std::shared_ptr<Player>& player, const std::shared_ptr<Maze>& maze, Ray rays[NUM_RAYS], float rayAngle, int stripId) const;
    float NormalizeAngle(float angle) const;
    float DistanceBetweenPoints(float x1, float y1, float x2, float y2) const;
};
