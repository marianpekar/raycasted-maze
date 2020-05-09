#pragma once

#include <iostream>
#include <SDL.h>
#include "Constants.h"

class Window
{
public:
	Window();
	~Window();
	SDL_Window* m_window;
};

