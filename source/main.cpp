#include <string>

#include "Game.h"
#include "Renderer.h"
#include "Window.h"

Window* window;
Renderer* renderer;
Game* game;

void ParseArgs(int argc, char** args, unsigned int& seed, bool& isBot)
{
    if (argc > 1)
    {
        for(size_t i = 0; i < argc; i++)
        {
            std::string arg = args[i];

            if (arg == "-s" || arg == "--seed")
            {
                seed = std::stoi(args[i + 1]);
            }
            else if (arg == "-b" || arg == "--bot")
            {
                isBot = true;
            }
        }
    }
}

int main(int argc, char* args[]) {

    unsigned int seed = 0;
    bool isBot = false;
    ParseArgs(argc, args, seed, isBot);
    
    window = new Window();
    renderer = new Renderer(window);
    game = new Game();

    game->Setup(seed, isBot);
	
    while (game->m_isGameRunning) {    	
        game->Update();
        renderer->Render(game);
        game->Delay(FRAME_TIME_LENGTH);
    }
	
    delete game;
    delete renderer;
    delete window;
    return 0;
}
