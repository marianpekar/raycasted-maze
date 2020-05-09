#include "Renderer.h"

Renderer::Renderer(SDL_Window* window)
{
    m_renderer = SDL_CreateRenderer(window, -1, 0);
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
        MINIMAP_SCALE_FACTOR * player->m_x + cos(player->m_rotationAngle) * 2,
        MINIMAP_SCALE_FACTOR * player->m_y + sin(player->m_rotationAngle) * 2
    );
}

void Renderer::RenderMinimap(Maze* maze) const
{
    for (int i = 0; i < MAZE_NUM_ROWS; i++) {
        for (int j = 0; j < MAZE_NUM_COLS; j++) {
            int tileX = j * TILE_SIZE;
            int tileY = i * TILE_SIZE;
            int tileColor = maze->m_maze[i][j] != 0 ? 255 : 0;

            SDL_SetRenderDrawColor(m_renderer, tileColor, tileColor, tileColor, 128);
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
    for (int i = 0; i < NUM_RAYS; i++) {
        SDL_RenderDrawLine(
            m_renderer,
            MINIMAP_SCALE_FACTOR * player->m_x,
            MINIMAP_SCALE_FACTOR * player->m_y,
            MINIMAP_SCALE_FACTOR * rays[i].m_wallHitX,
            MINIMAP_SCALE_FACTOR * rays[i].m_wallHitY
        );
    }
}

void Renderer::CleanColorBuffer(Uint32 color) const
{
    for (auto x = 0; x < WINDOW_WIDTH; x++) {
        for (auto y = 0; y < WINDOW_HEIGHT; y++) {
            m_colorBuffer[(WINDOW_WIDTH * y) + x] = color;
        }
    }
}

void Renderer::RenderColorBuffer() const
{
    SDL_UpdateTexture(
        m_colorBufferTexture,
        nullptr,
        m_colorBuffer,
        (int)((Uint32)WINDOW_WIDTH * sizeof(Uint32))
    );
    SDL_RenderCopy(m_renderer, m_colorBufferTexture, nullptr, nullptr);
}

void Renderer::RenderProjection(Player* player, Ray rays[], Surfaces* surfaces) const
{
    for (int i = 0; i < NUM_RAYS; i++) {
        float perpendicularDistance = rays[i].m_distance * cos(rays[i].m_rayAngle - player->m_rotationAngle);
        float distanceToProjectionPlane = (WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2);
        float projectedWallHeight = (TILE_SIZE / perpendicularDistance) * distanceToProjectionPlane;

        int wallStripHeight = (int)projectedWallHeight;

        int wallTopPixel = (WINDOW_HEIGHT / 2) - (wallStripHeight / 2);
        wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;

        int wallBottomPixel = (WINDOW_HEIGHT / 2) + (wallStripHeight / 2);
        wallBottomPixel = wallBottomPixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomPixel;

        for (int ceilingPixel = 1; ceilingPixel < wallTopPixel; ceilingPixel++) {
            m_colorBuffer[(WINDOW_WIDTH * ceilingPixel) + i] = 0xFF484848;
        }

        const auto surfaceId = rays[i].m_wallHitContent;

        const auto surfaceOffsetX = rays[i].m_wasHitVertical ? static_cast<int>(rays[i].m_wallHitY) % TILE_SIZE : static_cast<int>(rays[i].m_wallHitX) % TILE_SIZE;;

        for (auto y = wallTopPixel; y < wallBottomPixel; y++) {
            auto distanceFromTop = (y + (wallStripHeight / 2) - (WINDOW_HEIGHT / 2));
            auto surfaceOffetY = distanceFromTop * (static_cast<float>(TEXTURE_HEIGHT) / wallStripHeight);

            // set the color of the wall based on the color from the texture
            auto surface = surfaces->m_wallSurfaces[surfaceId];
            SDL_LockSurface(surface);

            uint32_t texel = Surfaces::GetTexel(surface, surfaceOffsetX, surfaceOffetY);

            SDL_UnlockSurface(surface);

            m_colorBuffer[(WINDOW_WIDTH * y) + i] = rays[i].m_wasHitVertical ? Surfaces::Darken(texel, 0.5f) : texel;
        }

        for (int floorPixel = wallBottomPixel; floorPixel < WINDOW_HEIGHT; floorPixel++) {
            m_colorBuffer[(WINDOW_WIDTH * floorPixel) + i] = 0xFF808080;
        }
    }
}

void Renderer::Render(Player* player, Maze* maze, Ray rays[], Surfaces* surfaces)
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    RenderProjection(player, rays, surfaces);

    RenderColorBuffer();
    CleanColorBuffer(0xFF000000);

    // minimap
    RenderMinimap(maze);
    RenderRays(player, rays);
    RenderPlayer(player);

    SDL_RenderPresent(m_renderer);
}
