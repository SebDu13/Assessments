#include <iostream>
#include <chrono>
#include <thread>

#include "PaperRockScissorsEngine.hpp"

namespace Game
{

PaperRockScissorsEngine::PaperRockScissorsEngine(std::shared_ptr<AbstractPlayer> player1, std::shared_ptr<AbstractPlayer> player2, bool activateAnimation)
: _player1(player1)
, _player2(player2)
, _activateAnimation(activateAnimation)
{
    if(!_player1 || !_player2)
        throw std::runtime_error("PaperRockScissorsEngine constructor failed, _player1/_player2 is null");
}

void PaperRockScissorsEngine::run(size_t roundCount)
{
    size_t currentRound = 1;
    if(roundCount == 0)
    {
        std::cout << "Number of round is 0, the game is already finished !" << std::endl;
        return;
    }

    std::cout << "*** Let's play paper rock scissors for " << roundCount << " rounds ***" << std::endl; 

    while(currentRound <= roundCount)
    {
        std::shared_ptr<AbstractPlayer> winner = _playRound();

        if(winner)
            std::cout << winner->getName() << " won this round " << currentRound << "/" << roundCount <<". ";
        else
            std::cout << "Draw! Round " << currentRound << "/" << roundCount <<". ";

        std::cout << _printScore() << std::endl;
        ++currentRound;
    }

    std::cout << "The game is finished, ";
    if(_player1->score == _player2->score)
        std::cout << " nobody lost! ";
    else
    {
        std::cout << (_player1->score > _player2->score ? _player1->getName() : _player2->getName()) << " won the game! ";
    }
    std::cout << _printScore() << std::endl;
}

std::shared_ptr<AbstractPlayer> PaperRockScissorsEngine::_playRound()
{
    auto symbol1 = _player1->openHand();
    auto symbol2 = _player2->openHand();

    if(_activateAnimation)
        PaperRockScissorsEngine::shiFuMiAnimation();

    std::cout << _player1->getName() << "---> " << symbol1.toStr() << " " << symbol2.toStr() << " <---" << _player2->getName() << std::endl;
    
    if(_activateAnimation)
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    if(symbol1 == symbol2)
        return nullptr;
    if(symbol1 > symbol2)
    {
        ++_player1->score;
        return _player1;
    }
    ++_player2->score;
    return _player2;
}

std::string PaperRockScissorsEngine::_printScore() const
{
    return "The score is " + _player1->printScore() + ", " + _player2->printScore();
}

void PaperRockScissorsEngine::shiFuMiAnimation(size_t delay)
{
    std::cout << "Shi...  ";
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    std::cout << "fu...  ";
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    std::cout << "mi!   ";
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}

}