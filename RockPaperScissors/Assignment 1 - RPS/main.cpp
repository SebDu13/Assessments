
#include <iostream>

#include "consolePlayer.hpp"
#include "computerPlayer.hpp"
#include "PaperRockScissorsEngine.hpp"

namespace helper
{
    std::optional<size_t> getRoundCount(const std::string& input)
    {
        try
        {
            return std::stoi(input);
        }
        catch(...)
        {
            return std::nullopt;
        }
    }
}

int main()
{
    Game::PaperRockScissorsEngine engine(std::make_shared<Game::ConsolePlayer>(), std::make_shared<Game::ComputerPlayer>());

    std::cout << "How many rounds do you want to play ?" << std::endl;
    std::string input;
    std::cin >> input;

    std::optional<size_t> roundCount;
    while(!(roundCount = helper::getRoundCount(input)))
    {
        std::cout << "Invalid input, please enter a number" << std::endl;
        std::cin >> input;
    }

    engine.run(*roundCount);
}
