#include <iostream>
#include "Renderer.h"
#include "Constants.h"
#include "Player.h"
#include "Maze.h"
#include "Ray.h"
#include "Surfaces.h"
#include "Window.h"
#include "Game.h"

Renderer::Renderer(Window* window)
{
    m_renderer = SDL_CreateRenderer(window->m_window, -1, 0);
    if (!m_renderer) {
        std::cout << "Error creating SDL renderer" << std::endl;
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

void Renderer::RenderPlayer(Player* player) const
{
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_Rect playerRect = {
        player->m_x * MINIMAP_SCALE_FACTOR,
        player->m_y * MINIMAP_SCALE_FACTOR,
        player->m_width * MINIMAP_SCALE_FACTOR,
        player->m_height * MINIMAP_SCALE_FACTOR
    };
    SDL_RenderFillRect(m_renderer, &playerRect);

    SDL_RenderDrawLine(
        m_renderer,
        MINIMAP_SCALE_FACTOR * player->m_x,
        MINIMAP_SCALE_FACTOR * player->m_y,
        MINIMAP_SCALE_FACTOR * player->m_x + static_cast<float>(cos(player->m_rotationAngle)) * 2,
        MINIMAP_SCALE_FACTOR * player->m_y + static_cast<float>(sin(player->m_rotationAngle)) * 2
    );
}

void Renderer::RenderMinimap(Maze* maze, Surfaces* surfaces) const
{
    for (int i = 0; i < MAZE_NUM_ROWS; i++) {
        for (int j = 0; j < MAZE_NUM_COLS; j++) {
            int tileX = j * TILE_SIZE;
            int tileY = i * TILE_SIZE;

            uint32_t tileColor = surfaces->m_minimapColors[maze->m_maze[i][j]];

            uint8_t r = (tileColor >> 16) & 0xFF;
            uint8_t g = (tileColor >> 8) & 0xFF;
            uint8_t b = tileColor & 0xFF;

            SDL_SetRenderDrawColor(m_renderer, r, g, b, 128);
            SDL_Rect mapTileRect = {
                tileX * MINIMAP_SCALE_FACTOR,
                tileY * MINIMAP_SCALE_FACTOR,
                TILE_SIZE * MINIMAP_SCALE_FACTOR,
                TILE_SIZE * MINIMAP_SCALE_FACTOR
            };
            SDL_RenderFillRect(m_renderer, &mapTileRect);
        }
    }
}

void Renderer::RenderRays(Player* player, Ray rays[]) const
{
    SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 128);
    for (auto i = 0; i < NUM_RAYS; i++) {
        SDL_RenderDrawLine(
            m_renderer,
            MINIMAP_SCALE_FACTOR * player->m_x,
            MINIMAP_SCALE_FACTOR * player->m_y,
            MINIMAP_SCALE_FACTOR * rays[i].m_wallHitX,
            MINIMAP_SCALE_FACTOR * rays[i].m_wallHitY
        );
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

void Renderer::RenderProjection(Player* player, Ray rays[], Surfaces* surfaces) const
{
    for (auto i = 0; i < NUM_RAYS; i++) {
	    const auto perpendicularDistance = rays[i].m_distance * cos(rays[i].m_rayAngle - player->m_rotationAngle);
	    const float distanceToProjectionPlane = (WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2);
	    const float projectedWallHeight = (TILE_SIZE / perpendicularDistance) * distanceToProjectionPlane;

	    const auto wallStripHeight = static_cast<int>(projectedWallHeight);

        auto wallTopPixel = (WINDOW_HEIGHT / 2) - (wallStripHeight / 2);
        wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;

        auto wallBottomPixel = (WINDOW_HEIGHT / 2) + (wallStripHeight / 2);
        wallBottomPixel = wallBottomPixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomPixel;

        for (auto ceilingPixel = 0; ceilingPixel <= wallTopPixel; ceilingPixel++) {
            m_colorBuffer[(WINDOW_WIDTH * ceilingPixel) + i] = CEILING_COLOR;
        }

        const auto surfaceId = rays[i].m_wallHitContent;

        const auto surfaceOffsetX = rays[i].m_wasHitVertical ? static_cast<int>(rays[i].m_wallHitY) % TILE_SIZE : static_cast<int>(rays[i].m_wallHitX) % TILE_SIZE;;

        for (auto y = wallTopPixel; y < wallBottomPixel; y++) {
	        const auto distanceFromTop = (y + (wallStripHeight / 2) - (WINDOW_HEIGHT / 2));
            const auto surfaceOffetY = distanceFromTop * (static_cast<float>(TEXTURE_HEIGHT) / wallStripHeight);

	        const auto texels = surfaces->m_textures[surfaceId];
	        const auto texel = Surfaces::GetPixel(texels, surfaceOffsetX, surfaceOffetY);
        	
            auto darkenAmount = rays[i].m_wasHitVertical ? perpendicularDistance * 0.001f : 0.3f + perpendicularDistance * 0.001f;
            if (darkenAmount > 0.6f)
                darkenAmount = 0.6f;

            m_colorBuffer[(WINDOW_WIDTH * y) + i] = Surfaces::Darken(texel, darkenAmount);
        }

        for (auto floorPixel = wallBottomPixel; floorPixel < WINDOW_HEIGHT; floorPixel++) {
            m_colorBuffer[(WINDOW_WIDTH * floorPixel) + i] = FLOOR_COLOR;
        }
    }
}

void Renderer::Render(Game* game)
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    RenderProjection(game->m_player, game->m_rays, game->m_surfaces);

    RenderColorBuffer();

    // minimap
    RenderMinimap(game->m_maze, game->m_surfaces);
    RenderRays(game->m_player, game->m_rays);
    RenderPlayer(game->m_player);

    SDL_RenderPresent(m_renderer);
}
