#include <iostream>
#include <random>

#include "computerPlayer.hpp"

namespace Game
{

Symbol ComputerPlayer::openHand() const
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, static_cast<int>(Symbol::Type::SYMBOL_SIZE)-1);

    auto symbol = Symbol::from(distr(gen));
    return *symbol;
};

std::string ComputerPlayer::getName() const
{
    return "COMPUTER";
}

}
