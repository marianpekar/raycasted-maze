#include <cmath>
#include <ctime>
#include <cstdlib>
#include "Game.h"
#include <memory>

#include "Maze.h"
#include "Node.h"
#include "PathFinder.h"
#include "Player.h"
#include "Raycaster.h"
#include "Surfaces.h"

void Game::Setup(unsigned int seed, bool isBot)
{
    std::srand(seed > 0 ? seed : std::time(0));

    Maze::GetRandomTile(m_startX, m_startY);

    m_maze = std::make_shared<Maze>(m_startX, m_startY);

    m_player = std::make_shared<Player>();
    m_player->isBot = isBot;
    m_player->x = m_startY * TILE_SIZE + TILE_SIZE * 0.5;
    m_player->y = m_startX * TILE_SIZE + TILE_SIZE * 0.5;
    m_player->width = 1;
    m_player->height = 1;
    m_player->turnDirection = 0;
    m_player->walkDirection = 0;
    m_player->rotationAngle = PI / 2;
    m_player->walkSpeed = PLAYER_WALK_SPEED;
    m_player->turnSpeed = PLAYER_TURN_SPEED * (PI / 180);

    m_raycaster = std::make_shared<Raycaster>();
    m_surfaces = std::make_shared<Surfaces>();
    
    m_isGameRunning = true;
}

void Game::Update()
{
    const auto deltaTime = (SDL_GetTicks() - m_ticksLastFrame) / 1000.0f;
    m_ticksLastFrame = SDL_GetTicks();
    
    if (m_player->isBot)
    {
        SimulateMovement(deltaTime);
    }
    else
    {
        GatherInput();
        MovePlayer(deltaTime);
    }
    
    m_raycaster->CastRays(m_player, m_maze, m_rays);
}

void Game::SimulateMovement(float deltaTime)
{
    if (m_currentNode == nullptr)
    {
        float x, y;
        m_maze->GetRandomOpenLocation(x,y);

        int targetTileX, targetTileY;
        m_maze->TileFromPosition(x, y, targetTileX, targetTileY);

        int playerTileX, playerTileY;
        m_maze->TileFromPosition(m_player->x, m_player->y, playerTileX, playerTileY);
    
        m_currentNode = PathFinder::FindPath(m_maze, playerTileX, playerTileY, targetTileX, targetTileY);
        m_currentNode = PathFinder::SimplifyPath(m_currentNode);
    }

    std::shared_ptr<Node> nextNode = m_currentNode->parent;
    if (nextNode == nullptr)
    {
        m_currentNode = nullptr;
        return;
    }
    
    float targetX = nextNode->x * TILE_SIZE + TILE_SIZE * 0.5f;
    float targetY = nextNode->y * TILE_SIZE + TILE_SIZE * 0.5f;
    float dx = targetX - m_player->x;
    float dy = targetY - m_player->y;

    float desiredAngle = std::atan2(dy, dx);

    float angleDiff = desiredAngle - m_player->rotationAngle;
    angleDiff = std::atan2(std::sin(angleDiff), std::cos(angleDiff));

    float turnFactor = 5.0f * deltaTime;  
    if (turnFactor > 1.0f)
        turnFactor = 1.0f;

    float newAngle = m_player->rotationAngle + angleDiff * turnFactor;
    newAngle = std::atan2(std::sin(newAngle), std::cos(newAngle));

    m_player->rotationAngle = newAngle;

    constexpr float angleThreshold = 0.004f;
    if (std::fabs(angleDiff) < angleThreshold)
    {
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance > 2.0f) 
        {
            float moveStep = m_player->walkSpeed * deltaTime;
            if (moveStep > distance)
                moveStep = distance;

            m_player->x += std::cos(m_player->rotationAngle) * moveStep;
            m_player->y += std::sin(m_player->rotationAngle) * moveStep;
        }
        else
        {
            m_currentNode = nextNode;
        }
    }
}

void Game::GatherInput()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT: {
            m_isGameRunning = false;
            break;
    }
    case SDL_KEYDOWN: {
            if (event.key.keysym.sym == SDLK_ESCAPE)
                m_isGameRunning = false;
            if (event.key.keysym.sym == SDLK_UP)
                m_player->walkDirection = +1;
            if (event.key.keysym.sym == SDLK_DOWN)
                m_player->walkDirection = -1;
            if (event.key.keysym.sym == SDLK_RIGHT)
                m_player->turnDirection = +1;
            if (event.key.keysym.sym == SDLK_LEFT)
                m_player->turnDirection = -1;
            if (event.key.keysym.sym == SDLK_SPACE)
                m_maze->GetRandomOpenLocation(m_player->x, m_player->y);
            break;
    }
    case SDL_KEYUP: {
            if (event.key.keysym.sym == SDLK_UP)
                m_player->walkDirection = 0;
            if (event.key.keysym.sym == SDLK_DOWN)
                m_player->walkDirection = 0;
            if (event.key.keysym.sym == SDLK_RIGHT)
                m_player->turnDirection = 0;
            if (event.key.keysym.sym == SDLK_LEFT)
                m_player->turnDirection = 0;
            break;
    }
    default:
        break;
    }
}

void Game::MovePlayer(float deltaTime) const
{
    m_player->rotationAngle += m_player->turnDirection * m_player->turnSpeed * deltaTime;
    auto moveStep = m_player->walkDirection * m_player->walkSpeed * deltaTime;

    const float newPlayerX = m_player->x + std::cos(m_player->rotationAngle) * moveStep;
    const float newPlayerY = m_player->y + std::sin(m_player->rotationAngle) * moveStep;

    if (!m_maze->HasWallAt(newPlayerX, newPlayerY)) {
        m_player->x = newPlayerX;
        m_player->y = newPlayerY;
    }
}

void Game::Delay(float time)
{
    SDL_Delay(time);
}
