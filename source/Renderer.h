#pragma once

#include <SDL.h>
#include <memory>

class Node;

class Renderer
{
public:
	Renderer(class Window* window);
	~Renderer();
	void Render(class Game* game);
private:
	uint32_t* m_colorBuffer;

	SDL_Renderer* m_renderer;
	SDL_Texture* m_colorBufferTexture;
	
	void RenderPlayer(class Player* player) const;
	void RenderMinimap(class Maze* maze, class Surfaces* surfaces) const;
	void RenderPath(std::shared_ptr<Node> path) const;
	void RenderRays(Player* player, struct Ray rays[]) const;
	void RenderColorBuffer() const;
	void RenderProjection(Player* player, Ray rays[], Surfaces* surfaces) const;
};
