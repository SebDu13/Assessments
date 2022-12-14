#pragma once

#include "abstractPlayer.hpp"
#include <optional>
#include<memory>

namespace Game
{

class PaperRockScissorsEngine
{
public:
    PaperRockScissorsEngine(std::shared_ptr<AbstractPlayer> player1, std::shared_ptr<AbstractPlayer> player2, bool activateAnimation = true);

    void run(size_t roundCount);

protected:
    std::shared_ptr<AbstractPlayer> _playRound();

private:
    std::string _printScore() const; 
    static void shiFuMiAnimation(size_t delay = 500);

    std::shared_ptr<AbstractPlayer> _player1;
    std::shared_ptr<AbstractPlayer> _player2;
    bool _activateAnimation;
};

}