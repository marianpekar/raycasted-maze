#include <cmath>
#include "Raycaster.h"

#include <memory>

#include "Player.h"
#include "Maze.h"
#include "Ray.h"

void Raycaster::CastRays(const std::shared_ptr<Player>& player, const std::shared_ptr<Maze>& maze, Ray rays[NUM_RAYS]) const
{
    float rayAngle = player->rotationAngle - (FOV_ANGLE / 2);

    for (int stripId = 0; stripId < NUM_RAYS; stripId++) {
        CastRay(player, maze, rays, rayAngle, stripId);
        rayAngle += FOV_ANGLE / NUM_RAYS;
    }
}

void Raycaster::CastRay(const std::shared_ptr<Player>& player, const std::shared_ptr<Maze>& maze, Ray rays[NUM_RAYS], float rayAngle, int stripId) const
{
    rayAngle = NormalizeAngle(rayAngle);

    const int isRayFacingDown = rayAngle > 0 && rayAngle < PI;
    const int isRayFacingUp = !isRayFacingDown;

    const int isRayFacingRight = rayAngle < 0.5 * PI || rayAngle > 1.5 * PI;
    const int isRayFacingLeft = !isRayFacingRight;

    int foundHorizontalWallHit = false;
    auto horizontalWallHitX = 0.0f;
    auto horizontalWallHitY = 0.0f;
    auto horizontalWallContent = 0;

    float yIntercept = floor(player->y / TILE_SIZE) * TILE_SIZE;
    yIntercept += isRayFacingDown ? TILE_SIZE : 0;

    float xIntercept = player->x + (yIntercept - player->y) / tan(rayAngle);

    float yStep = TILE_SIZE;
    yStep *= isRayFacingUp ? -1 : 1;

    float xStep = TILE_SIZE / tan(rayAngle);
    xStep *= (isRayFacingLeft && xStep > 0) ? -1 : 1;
    xStep *= (isRayFacingRight && xStep < 0) ? -1 : 1;

    auto nextHorizontalTouchX = xIntercept;
    auto nextHorizontalTouchY = yIntercept;

    while (nextHorizontalTouchX >= 0 && nextHorizontalTouchX <= MAZE_WIDTH * TILE_SIZE && nextHorizontalTouchY >= 0 && nextHorizontalTouchY <= MAZE_HEIGHT * TILE_SIZE) {
        auto xToCheck = nextHorizontalTouchX;
        auto yToCheck = nextHorizontalTouchY + (isRayFacingUp ? -1 : 0);

        if (maze->HasWallAt(xToCheck, yToCheck)) {
            horizontalWallHitX = nextHorizontalTouchX;
            horizontalWallHitY = nextHorizontalTouchY;
            horizontalWallContent = maze->GetAt(
                static_cast<int>(floor(xToCheck / TILE_SIZE)),
                static_cast<int>(floor(yToCheck / TILE_SIZE)));
            foundHorizontalWallHit = true;
            break;
        }
        nextHorizontalTouchX += xStep;
        nextHorizontalTouchY += yStep;
    }

    int foundVerticalWallHit = false;
    auto verticalWallHitX = 0.0f;
    auto verticalWallHitY = 0.0f;
    auto verticalWallContent = 0;

    xIntercept = floor(player->x / TILE_SIZE) * TILE_SIZE;
    xIntercept += isRayFacingRight ? TILE_SIZE : 0;

    yIntercept = player->y + (xIntercept - player->x) * tan(rayAngle);

    xStep = TILE_SIZE;
    xStep *= isRayFacingLeft ? -1 : 1;

    yStep = TILE_SIZE * tan(rayAngle);
    yStep *= (isRayFacingUp && yStep > 0) ? -1 : 1;
    yStep *= (isRayFacingDown && yStep < 0) ? -1 : 1;

    auto nextVerticalTouchX = xIntercept;
    auto nextVerticalTouchY = yIntercept;

    while (nextVerticalTouchX >= 0 && nextVerticalTouchX <= MAZE_WIDTH * TILE_SIZE && nextVerticalTouchY >= 0 && nextVerticalTouchY <= MAZE_HEIGHT * TILE_SIZE) {
	    const auto xToCheck = nextVerticalTouchX + (isRayFacingLeft ? -1 : 0);
	    const auto yToCheck = nextVerticalTouchY;

        if (maze->HasWallAt(xToCheck, yToCheck)) {
            // found a wall hit
            verticalWallHitX = nextVerticalTouchX;
            verticalWallHitY = nextVerticalTouchY;
            verticalWallContent = maze->GetAt(
                static_cast<int>(floor(xToCheck / TILE_SIZE)),
                static_cast<int>(floor(yToCheck / TILE_SIZE)));
            foundVerticalWallHit = true;
            break;
        }
	    nextVerticalTouchX += xStep;
	    nextVerticalTouchY += yStep;
    }

    const auto horizontalHitDistance = foundHorizontalWallHit ? DistanceBetweenPoints(player->x, player->y, horizontalWallHitX, horizontalWallHitY) : INT_MAX;
    const auto verticalHitDistance = foundVerticalWallHit ? DistanceBetweenPoints(player->x, player->y, verticalWallHitX, verticalWallHitY) : INT_MAX;

    if (verticalHitDistance < horizontalHitDistance) {
        rays[stripId].distance = verticalHitDistance;
        rays[stripId].wallHitX = verticalWallHitX;
        rays[stripId].wallHitY = verticalWallHitY;
        rays[stripId].wallHitContent = verticalWallContent;
        rays[stripId].wasHitVertical = true;
    }
    else {
        rays[stripId].distance = horizontalHitDistance;
        rays[stripId].wallHitX = horizontalWallHitX;
        rays[stripId].wallHitY = horizontalWallHitY;
        rays[stripId].wallHitContent = horizontalWallContent;
        rays[stripId].wasHitVertical = false;
    }
    rays[stripId].rayAngle = rayAngle;
    rays[stripId].isRayFacingDown = isRayFacingDown;
    rays[stripId].isRayFacingUp = isRayFacingUp;
    rays[stripId].isRayFacingLeft = isRayFacingLeft;
    rays[stripId].isRayFacingRight = isRayFacingRight;
}

float Raycaster::NormalizeAngle(float angle) const
{
    angle = std::remainder(angle, TWO_PI);
    if (angle < 0) {
        angle = TWO_PI + angle;
    }
    return angle;
}

float Raycaster::DistanceBetweenPoints(const float x1, const float y1, const float x2, const float y2) const
{
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
