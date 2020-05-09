#pragma once

#include <iostream>
#include <SDL.h>
#include "Constants.h"
#include "Player.h"
#include "Maze.h"
#include "Ray.h"
#include "Surfaces.h"

class Renderer
{
public:
	Renderer(SDL_Window* window);
	~Renderer();
	void Render(Player* player, Maze* maze, Ray rays[], Surfaces* surfaces);
private:
	SDL_Renderer* m_renderer;
	uint32_t* m_colorBuffer;
	SDL_Texture* m_colorBufferTexture;
	void RenderPlayer(Player* player) const;
	void RenderMinimap(Maze* maze) const;
	void RenderRays(Player* player, Ray rays[]) const;
	void CleanColorBuffer(Uint32 color) const;
	void RenderColorBuffer() const;
	void RenderProjection(Player* player, Ray rays[], Surfaces* surfaces) const;
};
