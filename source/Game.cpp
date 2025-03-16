#include <ctime>
#include <stdlib.h>
#include "Game.h"
#include "Maze.h"
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

void Game::Setup(unsigned int seed)
{
    srand(seed > 0 ? seed : time(0));

    m_startX = 1 + rand() % (MAZE_NUM_ROWS - 2);
    m_startY = 1 + rand() % (MAZE_NUM_COLS - 2);

    m_maze = new Maze(m_startX, m_startY);
    m_player = new Player;
    m_raycaster = new Raycaster();
    m_surfaces = new Surfaces();

    m_player->m_x = m_startY * TILE_SIZE + 1;
    m_player->m_y = m_startX * TILE_SIZE + 1;
    m_player->m_width = 1;
    m_player->m_height = 1;
    m_player->m_turnDirection = 0;
    m_player->m_walkDirection = 0;
    m_player->m_rotationAngle = PI / 2;
    m_player->m_walkSpeed = PLAYER_WALK_SPEED;
    m_player->m_turnSpeed = PLAYER_TURN_SPEED * (PI / 180);

    m_isGameRunning = true;
}

void Game::ProcessInput()
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

void Game::MovePlayer(float deltaTime)
{
    m_player->m_rotationAngle += m_player->m_turnDirection * m_player->m_turnSpeed * deltaTime;
    auto moveStep = m_player->m_walkDirection * m_player->m_walkSpeed * deltaTime;

    const float newPlayerX = m_player->m_x + cos(m_player->m_rotationAngle) * moveStep;
    const float newPlayerY = m_player->m_y + sin(m_player->m_rotationAngle) * moveStep;

    if (!m_maze->HasWallAt(newPlayerX, newPlayerY)) {
        m_player->m_x = newPlayerX;
        m_player->m_y = newPlayerY;
    }
}

void Game::Update()
{
    const auto deltaTime = (SDL_GetTicks() - m_ticksLastFrame) / 1000.0f;
    m_ticksLastFrame = SDL_GetTicks();

    MovePlayer(deltaTime);
    m_raycaster->CastRays(m_player, m_maze, m_rays);
}

void Game::Delay(float time)
{
    SDL_Delay(time);
}
