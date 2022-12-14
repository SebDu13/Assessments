#include "fixture.hpp"

namespace Test
{
using namespace Game;

PaperRockScissorsEngineTest::PaperRockScissorsEngineTest(std::shared_ptr<AbstractPlayer> player1, std::shared_ptr<AbstractPlayer> player2)
: PaperRockScissorsEngine(player1, player2, false)
{
}
std::shared_ptr<AbstractPlayer> PaperRockScissorsEngineTest::playRound()
{
    return _playRound();
}

Symbol PlayerTest::openHand() const
{
    return symbol;
}
std::string PlayerTest::getName() const
{
    return "";
}

}