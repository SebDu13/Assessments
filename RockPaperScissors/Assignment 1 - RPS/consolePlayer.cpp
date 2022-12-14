#include <iostream>
#include <optional>

#include "consolePlayer.hpp"

namespace helper
{
    std::optional<Game::Symbol> getSymbol(const std::string& input)
    {
        if(input.size() > 1 || input.empty())
            return std::nullopt;

        try
        {
            return Game::Symbol::from(std::stoi(input));
        }
        catch(...)
        {
            return std::nullopt;
        }
    }
}

namespace Game
{

Symbol ConsolePlayer::openHand() const
{
    std::cout << "Please pick a symbol: " << std::endl << Symbol::printSymbolWithIndex() << std::endl;
    
    std::string input;
    std::cin >> input;

    std::optional<Symbol> symbol;
    while(!(symbol = helper::getSymbol(input)))
    {
        std::cout << "Invalid input, please retry" << std::endl << Symbol::printSymbolWithIndex() << std::endl;
        std::cin >> input;
    }

    return *symbol;
   
};

std::string ConsolePlayer::getName() const
{
    return "PLAYER";
}

}
