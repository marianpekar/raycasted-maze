#include <iostream>
#include <ctime>
#include <SDL.h>
#include "Game.h"
#include "Renderer.h"
#include "Window.h"

Window* window;
Renderer* renderer;
Game* game;

int main(int argc, char* args[]) {
    window = new Window();
    renderer = new Renderer(window->m_window);
    game = new Game();

    game->Setup();
	
    while (game->m_isGameRunning) {
        game->ProcessInput();
        game->Update();
        renderer->Render(game->m_player, game->m_maze, game->m_rays, game->m_surfaces);
    }
	
    delete renderer;
    delete window;
    return 0;
}