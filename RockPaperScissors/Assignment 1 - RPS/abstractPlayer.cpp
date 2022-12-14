#include "abstractPlayer.hpp"

namespace Game
{
    
std::string AbstractPlayer::printScore() const
{
    return getName() + ":" + std::to_string(score);
}

}
