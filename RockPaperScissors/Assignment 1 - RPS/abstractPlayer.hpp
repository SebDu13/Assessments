#pragma once

#include "type.hpp"

namespace Game
{

class AbstractPlayer
{
public:
    virtual Symbol openHand() const = 0;
    virtual std::string getName() const = 0;

    std::string printScore() const;

    size_t score = 0;
};

}
