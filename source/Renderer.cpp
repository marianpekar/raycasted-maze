#include <iostream>
#include "Renderer.h"
#include "Constants.h"
#include "Player.h"
#include "Maze.h"
#include "Ray.h"
#include "Surfaces.h"
#include "Window.h"
#include "Game.h"
#include "Node.h"

Renderer::Renderer(const std::shared_ptr<Window>& window)
{
    m_renderer = SDL_CreateRenderer(window->m_window, -1, 0);
    if (!m_renderer) {
        std::cout << "Error creating SDL renderer\n";
    }

    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    m_colorBuffer = static_cast<uint32_t*>(malloc(sizeof(Uint32) * static_cast<Uint32>(WINDOW_WIDTH) * static_cast<Uint32>(WINDOW_HEIGHT)));

    m_colorBufferTexture = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );
}

Renderer::~Renderer()
{
    free(m_colorBuffer);
    SDL_DestroyRenderer(m_renderer);
}

void Renderer::RenderPlayer(const std::shared_ptr<Player>& player) const
{
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_Rect playerRect = {
        static_cast<int>(player->x * MINIMAP_SCALE_FACTOR),
        static_cast<int>(player->y * MINIMAP_SCALE_FACTOR),
        static_cast<int>(player->width * MINIMAP_SCALE_FACTOR),
        static_cast<int>(player->height * MINIMAP_SCALE_FACTOR)
    };
    SDL_RenderFillRect(m_renderer, &playerRect);

    SDL_RenderDrawLine(
        m_renderer,
        static_cast<int>(MINIMAP_SCALE_FACTOR * player->x),
        static_cast<int>(MINIMAP_SCALE_FACTOR * player->y),
        static_cast<int>(MINIMAP_SCALE_FACTOR * player->x + cos(player->rotationAngle) * 2.f),
        static_cast<int>(MINIMAP_SCALE_FACTOR * player->y + sin(player->rotationAngle) * 2.)
    );
}

void Renderer::RenderMinimap(const std::shared_ptr<Maze>& maze, const std::shared_ptr<Surfaces>& surfaces) const
{
    for (int i = 0; i < MAZE_HEIGHT; i++) {
        for (int j = 0; j < MAZE_WIDTH; j++) {
            float tileX = static_cast<float>(j) * TILE_SIZE;
            float tileY = static_cast<float>(i) * TILE_SIZE;

            uint32_t tileColor = surfaces->m_minimapColors[maze->GetAt(j,i)];

            uint8_t r = (tileColor >> 16) & 0xFF;
            uint8_t g = (tileColor >> 8) & 0xFF;
            uint8_t b = tileColor & 0xFF;

            SDL_SetRenderDrawColor(m_renderer, r, g, b, 128);
            SDL_Rect mapTileRect = {
                static_cast<int>(tileX * MINIMAP_SCALE_FACTOR),
                static_cast<int>(tileY * MINIMAP_SCALE_FACTOR),
                static_cast<int>(TILE_SIZE * MINIMAP_SCALE_FACTOR),
                static_cast<int>(TILE_SIZE * MINIMAP_SCALE_FACTOR)
            };
            SDL_RenderFillRect(m_renderer, &mapTileRect);
        }
    }
}

void Renderer::RenderRays(const std::shared_ptr<Player>& player, const Ray rays[]) const
{
    SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 128);
    for (auto i = 0; i < NUM_RAYS; i++) {
        SDL_RenderDrawLine(
            m_renderer,
            static_cast<int>(MINIMAP_SCALE_FACTOR * player->x),
            static_cast<int>(MINIMAP_SCALE_FACTOR * player->y),
            static_cast<int>(MINIMAP_SCALE_FACTOR * rays[i].wallHitX),
            static_cast<int>(MINIMAP_SCALE_FACTOR * rays[i].wallHitY)
        );
    }
}

void Renderer::RenderPath(const std::shared_ptr<Node>& path) const
{
    if (path == nullptr)
        return;
    
    std::shared_ptr<Node> currentNode = path;
    std::shared_ptr<Node> nextNode = currentNode->parent;
    while (currentNode != nullptr && nextNode != nullptr)
    {
        SDL_RenderDrawLine(
            m_renderer,
            static_cast<int>(MINIMAP_SCALE_FACTOR * static_cast<float>(currentNode->x) * TILE_SIZE),
            static_cast<int>(MINIMAP_SCALE_FACTOR * static_cast<float>(currentNode->y) * TILE_SIZE),
            static_cast<int>(MINIMAP_SCALE_FACTOR * static_cast<float>(nextNode->x) * TILE_SIZE),
            static_cast<int>(MINIMAP_SCALE_FACTOR * static_cast<float>(nextNode->y) * TILE_SIZE)
        );

        currentNode = nextNode;
        nextNode = currentNode->parent;
    }
}

void Renderer::RenderColorBuffer() const
{
    SDL_UpdateTexture(
        m_colorBufferTexture,
        nullptr,
        m_colorBuffer,
        static_cast<int>(static_cast<uint32_t>(WINDOW_WIDTH) * sizeof(uint32_t))
    );
    SDL_RenderCopy(m_renderer, m_colorBufferTexture, nullptr, nullptr);
}

void Renderer::RenderProjection(const std::shared_ptr<Player>& player, const Ray rays[], const std::shared_ptr<Surfaces>& surfaces) const
{
    for (int i = 0; i < NUM_RAYS; i++) {
	    const float perpendicularDistance = rays[i].distance * cos(rays[i].rayAngle - player->rotationAngle);
	    const float distanceToProjectionPlane = (WINDOW_WIDTH / 2.0f) / tan(FOV_ANGLE / 2);
	    const float projectedWallHeight = (TILE_SIZE / perpendicularDistance) * distanceToProjectionPlane;

        float wallTopPixel = (WINDOW_HEIGHT / 2.0f) - (projectedWallHeight / 2.0f);
        wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;

        float wallBottomPixel = (WINDOW_HEIGHT / 2.0f) + (projectedWallHeight / 2.0f);
        wallBottomPixel = wallBottomPixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomPixel;

        for (int ceilingPixel = 0; static_cast<float>(ceilingPixel) <= wallTopPixel; ceilingPixel++) {
            m_colorBuffer[(WINDOW_WIDTH * ceilingPixel) + i] = CEILING_COLOR;
        }

        const auto surfaceId = rays[i].wallHitContent;

        const auto surfaceOffsetX = rays[i].wasHitVertical ?
            static_cast<int>(rays[i].wallHitY) % static_cast<int>(TILE_SIZE) :
            static_cast<int>(rays[i].wallHitX) % static_cast<int>(TILE_SIZE);

        for (int y = static_cast<int>(wallTopPixel); y < static_cast<int>(wallBottomPixel); y++) {
	        const float distanceFromTop = static_cast<float>(y) + (projectedWallHeight / 2) - (WINDOW_HEIGHT / 2.0f);
            const float surfaceOffetY = distanceFromTop * (static_cast<float>(TEXTURE_HEIGHT) / projectedWallHeight);

	        const auto texels = surfaces->m_textures[surfaceId];
	        const uint32_t texel = Surfaces::GetPixel(texels, surfaceOffsetX, static_cast<int>(surfaceOffetY));
        	
            float darkenAmount = rays[i].wasHitVertical ? perpendicularDistance * 0.001f : 0.3f + perpendicularDistance * 0.001f;
            if (darkenAmount > 0.6f)
                darkenAmount = 0.6f;

            m_colorBuffer[(WINDOW_WIDTH * y) + i] = Surfaces::Darken(texel, darkenAmount);
        }

        for (int floorPixel = static_cast<int>(wallBottomPixel); floorPixel < WINDOW_HEIGHT; floorPixel++) {
            m_colorBuffer[(WINDOW_WIDTH * floorPixel) + i] = FLOOR_COLOR;
        }
    }
}

void Renderer::Render(const std::shared_ptr<Game>& game) const
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    RenderProjection(game->GetPlayer(), game->GetRays(), game->GetSurfaces());

    RenderColorBuffer();

    // minimap
    RenderMinimap(game->GetMaze(), game->GetSurfaces());
    RenderRays(game->GetPlayer(), game->GetRays());
    RenderPlayer(game->GetPlayer());
    RenderPath(game->GetCurrentNode());

    SDL_RenderPresent(m_renderer);
}
