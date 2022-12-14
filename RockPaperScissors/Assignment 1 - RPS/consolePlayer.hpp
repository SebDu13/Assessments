#pragma once
#include "abstractPlayer.hpp"

namespace Game
{

class ConsolePlayer: public AbstractPlayer
{
public:
    Symbol openHand() const override;
    std::string getName() const override;
};

}
