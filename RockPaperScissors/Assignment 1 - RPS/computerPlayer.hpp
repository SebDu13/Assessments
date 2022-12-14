#pragma once
#include "abstractPlayer.hpp"

namespace Game
{

class ComputerPlayer: public AbstractPlayer
{
public:
    Symbol openHand() const override;
    std::string getName() const override;
};

}
