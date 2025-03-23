#include <string>
#include <memory>

#include "Game.h"
#include "Renderer.h"
#include "Window.h"

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
    
    const std::shared_ptr<Window> window = std::make_shared<Window>();
    const std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(window);
    const std::shared_ptr<Game> game = std::make_shared<Game>();

    game->Setup(seed, isBot);
	
    while (game->IsRunning()) {    	
        game->Update();
        renderer->Render(game);
        game->Delay(FRAME_TIME_LENGTH);
    }
    
    return 0;
}
