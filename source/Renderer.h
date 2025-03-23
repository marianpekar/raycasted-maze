#pragma once

#include <SDL.h>
#include <memory>

struct Ray;
struct Node;
class Maze;
class Surfaces;
struct Player;

class Window;
class Game;

class Renderer
{
public:
	Renderer(const std::shared_ptr<Window>& window);
	~Renderer();
	void Render(const std::shared_ptr<Game>& game) const;
private:
	uint32_t* m_colorBuffer;

	SDL_Renderer* m_renderer;
	SDL_Texture* m_colorBufferTexture;
	
	void RenderPlayer(const std::shared_ptr<Player>& player) const;
	void RenderMinimap(const std::shared_ptr<Maze>& maze, const std::shared_ptr<Surfaces>& surfaces) const;
	void RenderPath(const std::shared_ptr<Node>& path) const;
	void RenderRays(const std::shared_ptr<Player>& player, const Ray rays[]) const;
	void RenderColorBuffer() const;
	void RenderProjection(const std::shared_ptr<Player>& player, const Ray rays[], const std::shared_ptr<Surfaces>& surfaces) const;
};
