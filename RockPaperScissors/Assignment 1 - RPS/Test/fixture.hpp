#pragma once
#include "../PaperRockScissorsEngine.hpp"
#include "../abstractPlayer.hpp"

namespace Test
{
 using namespace Game;

class PaperRockScissorsEngineTest : public PaperRockScissorsEngine
{
public:
    PaperRockScissorsEngineTest(std::shared_ptr<AbstractPlayer> player1, std::shared_ptr<AbstractPlayer> player2);
    std::shared_ptr<AbstractPlayer> playRound();
};

class PlayerTest : public AbstractPlayer
{
public:
    Symbol openHand() const override;
    std::string getName() const override;
    Symbol symbol = Symbol::Type::SYMBOL_SIZE;
};
    
}