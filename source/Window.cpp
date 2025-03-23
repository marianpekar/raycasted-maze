#include <SDL.h>
#include "Window.h"
#include <iostream>
#include "Constants.h"

Window::Window()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Error initializing SDL.\n";
    }
    m_window = SDL_CreateWindow(
        nullptr,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );
    if (!m_window) {
        std::cout << "Error creating SDL window.\n";
    }
}

Window::~Window()
{
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
