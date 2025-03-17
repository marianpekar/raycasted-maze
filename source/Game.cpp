#include <cmath>
#include <ctime>
#include <cstdlib>
#include "Game.h"
#include "Maze.h"
#include "Node.h"
#include "PathFinder.h"
#include "Player.h"
#include "Raycaster.h"
#include "Surfaces.h"

Game::~Game()
{
    delete m_surfaces;
    delete m_raycaster;
    delete m_player;
    delete m_maze;
}

void Game::Setup(unsigned int seed, bool isBot)
{
    srand(seed > 0 ? seed : time(0));

    Maze::GetRandomTile(m_startX, m_startY);

    m_maze = new Maze(m_startX, m_startY);

    m_player = new Player;
    m_player->m_isBot = isBot;
    m_player->m_x = m_startY * TILE_SIZE + TILE_SIZE * 0.5;
    m_player->m_y = m_startX * TILE_SIZE + TILE_SIZE * 0.5;
    m_player->m_width = 1;
    m_player->m_height = 1;
    m_player->m_turnDirection = 0;
    m_player->m_walkDirection = 0;
    m_player->m_rotationAngle = PI / 2;
    m_player->m_walkSpeed = PLAYER_WALK_SPEED;
    m_player->m_turnSpeed = PLAYER_TURN_SPEED * (PI / 180);

    m_raycaster = new Raycaster();
    m_surfaces = new Surfaces();
    
    m_isGameRunning = true;
}

void Game::Update()
{
    const auto deltaTime = (SDL_GetTicks() - m_ticksLastFrame) / 1000.0f;
    m_ticksLastFrame = SDL_GetTicks();
    
    if (m_player->m_isBot)
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
        m_maze->TileFromPosition(m_player->m_x, m_player->m_y, playerTileX, playerTileY);
    
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
    float dx = targetX - m_player->m_x;
    float dy = targetY - m_player->m_y;

    float desiredAngle = std::atan2(dy, dx);

    float angleDiff = desiredAngle - m_player->m_rotationAngle;
    angleDiff = std::atan2(std::sin(angleDiff), std::cos(angleDiff));

    float turnFactor = 5.0f * deltaTime;  
    if (turnFactor > 1.0f)
        turnFactor = 1.0f;

    float newAngle = m_player->m_rotationAngle + angleDiff * turnFactor;
    newAngle = std::atan2(std::sin(newAngle), std::cos(newAngle));

    m_player->m_rotationAngle = newAngle;

    constexpr float angleThreshold = 0.004f;
    if (std::fabs(angleDiff) < angleThreshold)
    {
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance > 2.0f) 
        {
            float moveStep = m_player->m_walkSpeed * deltaTime;
            if (moveStep > distance)
                moveStep = distance;

            m_player->m_x += std::cos(m_player->m_rotationAngle) * moveStep;
            m_player->m_y += std::sin(m_player->m_rotationAngle) * moveStep;
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
                m_player->m_walkDirection = +1;
            if (event.key.keysym.sym == SDLK_DOWN)
                m_player->m_walkDirection = -1;
            if (event.key.keysym.sym == SDLK_RIGHT)
                m_player->m_turnDirection = +1;
            if (event.key.keysym.sym == SDLK_LEFT)
                m_player->m_turnDirection = -1;
            if (event.key.keysym.sym == SDLK_SPACE)
                m_maze->GetRandomOpenLocation(m_player->m_x, m_player->m_y);
            break;
    }
    case SDL_KEYUP: {
            if (event.key.keysym.sym == SDLK_UP)
                m_player->m_walkDirection = 0;
            if (event.key.keysym.sym == SDLK_DOWN)
                m_player->m_walkDirection = 0;
            if (event.key.keysym.sym == SDLK_RIGHT)
                m_player->m_turnDirection = 0;
            if (event.key.keysym.sym == SDLK_LEFT)
                m_player->m_turnDirection = 0;
            break;
    }
    default:
        break;
    }
}

void Game::MovePlayer(float deltaTime) const
{
    m_player->m_rotationAngle += m_player->m_turnDirection * m_player->m_turnSpeed * deltaTime;
    auto moveStep = m_player->m_walkDirection * m_player->m_walkSpeed * deltaTime;

    const float newPlayerX = m_player->m_x + std::cos(m_player->m_rotationAngle) * moveStep;
    const float newPlayerY = m_player->m_y + std::sin(m_player->m_rotationAngle) * moveStep;

    if (!m_maze->HasWallAt(newPlayerX, newPlayerY)) {
        m_player->m_x = newPlayerX;
        m_player->m_y = newPlayerY;
    }
}

void Game::Delay(float time)
{
    SDL_Delay(time);
}
