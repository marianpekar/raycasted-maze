#include "Window.h"

Window::Window()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Error initializing SDL." << std::endl;
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
        std::cout << "Error creating SDL window." << std::endl;
    }
}

Window::~Window()
{
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
